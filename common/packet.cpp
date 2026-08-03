#include "common/packet.hpp"

#include <utility>
#include <vector>
#include <array>
#include <cstring>
#include <stdexcept>
#include <optional>
#include <arpa/inet.h>

#include <boost/asio.hpp>

namespace scalable {
namespace common {

    // OutgoingPacket
    struct OutgoingPacket
    {
        std::vector<uint8_t> fields;
        std::vector<uint8_t> headers;
        std::vector<uint8_t> payload;
        
        std::array<boost::asio::const_buffer, 3> 
        buffers() const
        {
            return {
                boost::asio::buffer(fields),
                boost::asio::buffer(headers),
                boost::asio::buffer(payload)
            };
        }
    };

    Packet::Packet(std::vector<uint8_t>&& bytes) :
    buffer_(std::move(bytes))
    {
        packet_size_=static_cast<uint32_t>(buffer_.size());
        // Packet max size is validated in the Reader class
        if(packet_size_ < offset_headers_)
            throw std::invalid_argument("Packet size error");

        version_=packet_version_from_byte(buffer_[offset_version_]);
        if(version_==PacketVersion::Unknown)
            throw std::invalid_argument("Version Error");

        packet_type_=packet_type_from_byte(buffer_[offset_packet_type_]);
        if(packet_type_==PacketType::Unknown)
            throw std::invalid_argument("packet type error");

        std::memcpy(&header_size_, buffer_.data()+offset_header_size_, sizeof(header_size_));
        header_size_=ntohs(header_size_);
        if(header_size_ > MAX_HEADER_SIZE || 
            header_size_ > packet_size_)
            throw std::invalid_argument("Header size error");

        uint16_t flags=0;
        std::memcpy(&flags, buffer_.data()+offset_flags_, sizeof(flags));
        flags=ntohs(flags);
        flags_=packet_flags_from_bytes(flags);
        if(flags_!=PacketFlags::None)
            throw std::invalid_argument("Packet flags error");

        std::memcpy(&sequence_id_, buffer_.data()+offset_sequence_id_, sizeof(sequence_id_));
        sequence_id_=ntohl(sequence_id_);

        offset_payload_ = offset_headers_ + header_size_;
        if(offset_payload_ > packet_size_)
            throw std::invalid_argument("Offset payload error");

        payload_size_ = packet_size_ - offset_payload_;
        if(payload_size_ > MAX_PAYLOAD_SIZE)
            throw std::invalid_argument("Payload size error");
            
        parseSubHeaders();
    }

    OutgoingPacket Packet::create(
        PacketVersion version,
        PacketType type,
        PacketFlags flags,
        uint32_t sequence_id,
        std::vector<uint8_t>&& headers,
        std::vector<uint8_t>&& payload)
    {
        uint8_t version_ = 0;
        if(version == PacketVersion::Unknown)
            throw std::invalid_argument("Invalid packet version");
        version_ = Packet::packet_version_to_byte(version);

        uint8_t type_ = 0;
        if(type == PacketType::Unknown)
            throw std::invalid_argument("Invalid packet type");
        type_ = Packet::packet_type_to_byte(type);

        if(headers.size() > MAX_HEADER_SIZE)
            throw std::invalid_argument("Header size exceeds the maximum allowed");
        uint16_t header_size_ = static_cast<uint16_t>(headers.size());

        uint16_t flags_ = 0;
        if(flags != PacketFlags::None)
            throw std::invalid_argument("Invalid packet flags");
        flags_ = Packet::packet_flags_to_bytes(flags);

        //uint32_t sequence_id_ = sequence_id;

        if(payload.size() > MAX_PAYLOAD_SIZE)
            throw std::invalid_argument("Payload size exceeds the maximum allowed");
        uint32_t payload_size_ = static_cast<uint32_t>(payload.size());
        
        uint32_t packet_size_ = 
            static_cast<uint32_t>(Packet::offset_headers_ + header_size_ + payload_size_);
   
        packet_size_ = htonl(packet_size_);
        //version 1 byte does not have endain
        //packet_type 1 byte does not have endain
        header_size_ = htons(header_size_);
        flags_ = htons(flags_);
        sequence_id = htonl(sequence_id);
        
        std::vector<uint8_t> fields(Packet::offset_headers_);
        std::memcpy(fields.data(), &packet_size_, sizeof(packet_size_));
        std::memcpy(fields.data() + Packet::offset_packet_type_, &type_, sizeof(type_));
        std::memcpy(fields.data() + Packet::offset_version_, &version_, sizeof(version_));
        std::memcpy(fields.data() + Packet::offset_header_size_, &header_size_, sizeof(header_size_));
        std::memcpy(fields.data() + Packet::offset_flags_, &flags_, sizeof(flags_));
        std::memcpy(fields.data() + Packet::offset_sequence_id_, &sequence_id, sizeof(sequence_id));

        OutgoingPacket packet;
        packet.fields = std::move(fields);
        packet.headers = std::move(headers);
        packet.payload = std::move(payload);
        
        return packet;
    }

    // subHeader
    void Packet::parseSubHeaders()
    {

        uint16_t count = 0;
        
        while(count < header_size_)
        {
            uint16_t sub_header_size = 0;
            uint8_t sub_header_type = 0;

            if(header_size_ - count < sizeof(sub_header_size))
            {
                throw std::invalid_argument("Incomplete SubHeaderSize");
            }

            std::memcpy(&sub_header_size, 
                buffer_.data() + offset_headers_ + count ,
                sizeof(sub_header_size));
            
            if(sub_header_size > MAX_HEADER_SIZE)
            {
                throw std::invalid_argument("Invalid sub header size");
            }

            count += sizeof(sub_header_size);

            if( header_size_ - count < sizeof(sub_header_type) )
            {
                throw std::invalid_argument("Incomplete SubHeaderType");
            }

            std::memcpy(&sub_header_type, 
                buffer_.data() + offset_headers_ + count ,
                sizeof(sub_header_type));

            if(sub_header_from_byte(sub_header_type) == SubHeaderType::Unknown)
            {
                throw std::invalid_argument("Invalid SubHeaderType");
            }

            count += sizeof(sub_header_type);

            if(header_size_ - count < sub_header_size)
            {
                throw std::invalid_argument("Incomplete SubHeaderValue");
            }

            SubHeader sub;
            sub.type = sub_header_from_byte(sub_header_type);
            sub.size = sub_header_size;
            //sub.offset = offset_headers_ + count;
            sub.value = std::span<const uint8_t>(
                buffer_.data() + offset_headers_ + count,
                sub_header_size);
            headers_.push_back(std::move(sub));

            count += sub_header_size;

        }

        if(count != header_size_)
        {
            throw std::invalid_argument("Invalid header size");
        }
    } 

    std::optional<uint8_t> Packet::sub_header_get_uint8(SubHeaderType type) const
    {
        
        uint8_t value = 0;
        for(const auto& sub: headers_)
        {
            if(sub.type==type)
            {
                if(sub.value.size() != sizeof(uint8_t))
                    return std::nullopt;

                std::memcpy(&value, sub.value.data(), sizeof(value));
                return std::optional<uint8_t>(value);
            }
        }

        return std::nullopt;
    }

    std::optional<uint32_t> Packet::sub_header_get_uint32(SubHeaderType type) const
    {
        
        uint32_t value = 0;
        for(const auto& sub: headers_)
        {
            if(sub.type==type)
            {
                if(sub.value.size() != sizeof(uint32_t))
                    return std::nullopt;

                std::memcpy(&value, sub.value.data(), sizeof(value));
                value=ntohl(value);
                return std::optional<uint32_t>(value);
            }
        }

        return std::nullopt;
    }

    std::optional<std::string> Packet::sub_eader_get_string(SubHeaderType type) const
    {
        for(const auto& sub:headers_)
        {
            if(sub.type==type)
            {
                std::string value(
                    reinterpret_cast<const char*>(sub.value.data()),
                    sub.value.size()); 

                return std::optional<std::string>(value);
            }
        }
        return std::nullopt;
    }


    uint8_t Packet::sub_header_to_byte(SubHeaderType type)
    {
        switch (type)
        {
        case SubHeaderType::SessionId:
            return 10;
        case SubHeaderType::UserId:
            return 11;
        case SubHeaderType::AuthToken:
            return 12;
        
        default:
            return 0;
        }
    }

    SubHeaderType Packet::sub_header_from_byte(uint8_t byte)
    {
        switch (byte)
        {
        case 10:
            return SubHeaderType::SessionId;
        case 11:
            return SubHeaderType::UserId;
        case 12:
            return SubHeaderType::AuthToken;
        
        default:
            return SubHeaderType::Unknown;
        }
    }

    // Getters

    uint32_t Packet::packet_size() const{ return packet_size_; }
    PacketVersion Packet::version() const{ return version_; }
    PacketType Packet::packet_type() const{ return packet_type_; }
    uint16_t Packet::header_size() const{ return header_size_; }
    PacketFlags Packet::flags() const{ return flags_; }
    uint32_t Packet::sequence_id() const{ return sequence_id_; }
    uint32_t Packet::payload_size() const{ return payload_size_; }

    const uint8_t* Packet::headers() const { 
        return buffer_.data()+offset_headers_; 
    };

    const uint8_t* Packet::payload() const {
        return buffer_.data()+offset_payload_;
    };

    // Convertors

    uint8_t Packet::packet_version_to_byte(PacketVersion version)
    {
        switch (version)
        {
        case PacketVersion::v1:
            return 1;
        
        default:
            return 0;
        }
    }

    PacketVersion Packet::packet_version_from_byte(uint8_t version)
    {
        switch (version)
        {
        case 1:
            return PacketVersion::v1;
        default:
            return PacketVersion::Unknown;
        }
    }

    uint8_t Packet::packet_type_to_byte(PacketType type)
    {
        switch(type)
        {
            case PacketType::Text:
                return 1;
            case PacketType::File:
              return 2;
            case PacketType::Video:
              return 3;
            default:
              return 0;
        }
    }

    PacketType Packet::packet_type_from_byte(uint8_t type)
    {
        switch (type)
        {
        case 1:
            return PacketType::Text;
        case 2:
            return PacketType::File;
        case 3:
            return PacketType::Video;
        default:
            return PacketType::Unknown;
        }
    }

    uint16_t Packet::packet_flags_to_bytes(PacketFlags flags)
    {
        return 0;
    }

    PacketFlags Packet::packet_flags_from_bytes(uint16_t flags)
    {
        return PacketFlags::None;
    }
    
}
}