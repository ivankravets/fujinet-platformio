/**
 * NetworkProtocolDFS 
 * 
 * implementation
 */

#include "DFS.h"

NetworkProtocolDFS::NetworkProtocolDFS(string *rx_buf, string *tx_buf, string *sp_buf)
    : NetworkProtocol(rx_buf, tx_buf, sp_buf)
{
}

NetworkProtocolDFS::~NetworkProtocolDFS()
{
}

bool NetworkProtocolDFS::open(EdUrlParser *url, cmdFrame_t *cmdFrame)
{
    return true;
}

bool NetworkProtocolDFS::close()
{
    return false;
}

bool NetworkProtocolDFS::read(unsigned short len)
{
    return true;
}

bool NetworkProtocolDFS::write(unsigned short len)
{
    return true;
}

bool NetworkProtocolDFS::status(NetworkStatus *status)
{
    return true;
}

uint8_t NetworkProtocolDFS::special_inquiry(uint8_t cmd)
{
    return 0xFF;
}

bool NetworkProtocolDFS::special_00(cmdFrame_t *cmdFrame)
{
    return true;
}

bool NetworkProtocolDFS::special_40(uint8_t *sp_buf, unsigned short len, cmdFrame_t *cmdFrame)
{
    return true;
}

bool NetworkProtocolDFS::special_80(uint8_t *sp_buf, unsigned short len, cmdFrame_t *cmdFrame)
{
    return true;
}

bool NetworkProtocolDFS::perform_idempotent_80(EdUrlParser *url, cmdFrame_t *cmdFrame)
{
    return true;
}
