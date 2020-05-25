#include "networkProtocolFTP.h"
#include "../../include/debug.h"

bool networkProtocolFTP::ftpExpect(string resultCode)
{
    char buf[512];
    string sbuf;

    memset(buf, 0, sizeof(buf));

    if (!control.connected())
        return false;

    control.setTimeout(5);

    delay(250);

    int l = control.readBytesUntil('\n', buf, sizeof(buf));
    Debug_printf("We got %d bytes back\n", l);

    if (l > 4)
    {
        sbuf = string(buf);
        controlResponse = sbuf.substr(4);
    }
    else
    {
        return false;
    }

    Debug_printf("Got response: %s\n", buf);
    Debug_printf("Returning response: %s\n", controlResponse.c_str());
    return (sbuf.find_first_of(resultCode) == 0 ? true : false);
}

unsigned short networkProtocolFTP::parsePort(string response)
{
    size_t pos_start = 0;
    size_t pos_end = 0;
    unsigned short port;

    pos_start = response.find_first_of("|");
    pos_start += 2;

    Debug_printf("pos_start %d\n", pos_start);

    pos_end = response.find_last_of("|");
    Debug_printf("pos_end %d\n", pos_end);
    pos_start++;
    pos_end--;
    port = (atoi(response.substr(pos_start, pos_end).c_str()));
    Debug_printf("port string %s\r\n", response.substr(pos_start, pos_end).c_str());
    Debug_printf("Parsed port is: %d\n", port);
    return port;
}

networkProtocolFTP::networkProtocolFTP()
{
}

networkProtocolFTP::~networkProtocolFTP()
{
}

bool networkProtocolFTP::open(EdUrlParser *urlParser, cmdFrame_t *cmdFrame)
{
    string tmpPath;
    string tmpChdirPath;

    if (urlParser->port.empty())
        urlParser->port = "21";

    hostName = urlParser->hostName;

    if (!control.connect(urlParser->hostName.c_str(), atoi(urlParser->port.c_str())))
        return false; // Error

    Debug_printf("Waiting for banner.\n");

    if (!ftpExpect("220"))
        return false; // error

    Debug_printf("Got user, sending USER.\n");

    control.write("USER anonymous\r\n");

    if (!ftpExpect("331"))
        return false;

    Debug_printf("User Ok, sending password.\n");

    control.write("PASS fujinet@fujinet.online\r\n");

    Debug_printf("Logged in.\n");

    if (!ftpExpect("230"))
        return false;

    Debug_printf("Setting type to IMAGE\n");

    control.write("TYPE I\r\n");

    if (!ftpExpect("200"))
        return false;

    tmpPath = urlParser->path.substr(0, urlParser->path.find("*") - 1);

    Debug_printf("TYPE I OK. Attempting to CWD to %s\n", tmpPath.c_str());

    control.write("CWD ");
    control.write(tmpPath.c_str());
    control.write("\r\n");

    if (!ftpExpect("250"))
    {
        string tmp = tmpPath;

        // Trim off last part of filename, hopefully to just a dir path
        tmp = tmp.substr(0, tmp.find_last_of("/"));

        Debug_printf("Workaround, trying again... with %s \n", tmp.c_str());

        // and try again.
        control.write("CWD ");
        control.write(tmp.c_str());
        control.write("\r\n");

        if (!ftpExpect("250"))
            return false; // Still can't find.
    }

    aux1 = cmdFrame->aux1;

    Debug_printf("Attempting to get passive port\n");
    control.write("EPSV\r\n");

    if (!ftpExpect("229"))
        return false;

    dataPort = parsePort(controlResponse);
    Debug_printf("Received EPSV response. Port %d\n", dataPort);

    switch (cmdFrame->aux1)
    {
    case 4:
        Debug_printf("Attempting to open RETR. to %s\n", urlParser->path.c_str());

        control.write("RETR ");
        control.write(urlParser->path.c_str());
        control.write("\r\n");
        break;
    case 6:
        tmpPath = urlParser->path.substr(urlParser->path.find_last_of("/") + 1);
        Debug_printf("Attempting NLST to %s", tmpPath.c_str());
        control.write("NLST");
        if ((tmpPath != "*.*") && (tmpPath != "*") && (tmpPath != "**.*") && (tmpPath != "**"))
        {
            control.write(" ");
            control.write(tmpPath.c_str());
        }
        control.write("\r\n");
        break;
    case 8:
        Debug_printf("Storing file %s\n", urlParser->path.c_str());
        control.write("STOR ");
        control.write(urlParser->path.c_str());
        control.write("\r\n");
        break;
    default:
        Debug_printf("Unimplemented aux1 = %d", cmdFrame->aux1);
        return false;
    }

    if (!data.connect(hostName.c_str(), dataPort))
        return false;

    delay(500);

    Debug_printf("Connected to data port: %d", dataPort);

    return true;
}

bool networkProtocolFTP::close()
{
    if (data.connected())
        data.stop();

    if (control.connected())
    {
        Debug_printf("Connected to data port, closing it.\n");
        ftpExpect("150");
        ftpExpect("226");
        control.write("QUIT\r\n");
        ftpExpect("221");
    }

    control.stop();
    return true;
}

bool networkProtocolFTP::read(byte *rx_buf, unsigned short len)
{
    if (data.readBytes(rx_buf, len) != len)
        return true;
    else
        dataSize -= len;

    if (aux1 == 6)
    {
        for (int i = 0; i < len; i++)
        {
            if (rx_buf[i] == 0x0D)
                rx_buf[i] = 0x20;
            else if (rx_buf[i] == 0x0A)
                rx_buf[i] = 0x9B;
        }
    }
    return false;
}

bool networkProtocolFTP::write(byte *tx_buf, unsigned short len)
{
    if (data.write(tx_buf, len) != len)
        return true;

    return false;
}

bool networkProtocolFTP::status(byte *status_buf)
{
    status_buf[0] = data.available() & 0xFF;
    status_buf[1] = data.available() >> 8;
    status_buf[2] = 1;
    status_buf[3] = 1;
    return false;
}

bool networkProtocolFTP::special(byte *sp_buf, unsigned short len, cmdFrame_t *cmdFrame)
{
    return false;
}

bool networkProtocolFTP::special_supported_00_command(unsigned char comnd)
{
    return false;
}