#pragma once

/*

+----------------+----------+---------------+--------------+
| packet_size    | version  | message_type  | header_size  |
| 4 bytes        | 1 byte   | 1 byte        | 2 bytes      |
+----------------+----------+---------------+--------------+
| flags          | sequence_id                             |
| 2 bytes        | 4 bytes                                 |
+----------------------------------------------------------+
|                                                          |
| headers (variable)                                       |
|                                                          |
+----------------------------------------------------------+
|                                                          |
| payload (variable)                                       |
|                                                          |
+----------------------------------------------------------+

*/


#include <string>
#include <vector>
#include "packet_type.hpp"

namespace scalable {
namespace common {
    
class Packet
{
public:
    
private:

};


}
}