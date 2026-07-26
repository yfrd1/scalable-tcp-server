#pragma once

#include <cstdint>

namespace scalable {
namespace common {

    enum class SubHeaderType : uint8_t
    {
        Unknown = 0,
        
        // Authentication / Identity
        SessionId,
        UserId,
        AuthToken,

        /*
        // Request / Response
        RequestId,
        CorrelationId,

        // Content
        ContentType,
        ContentLength,
        ContentEncoding,

        // Chat
        RoomId,
        MessageId,
        ReplyToMessageId,

        // File Transfer
        FileId,
        FileName,
        FileSize,
        FileOffset,
        ChunkId,
        ChunkSize,

        // Streaming
        StreamId,
        StreamType,
        FrameId,
        FrameType,
        Timestamp,

        // Connection / Protocol
        ClientId,
        DeviceId,
        Compression,

        // Optional / Future
        Language,
        Metadata
        */
    };

}
}