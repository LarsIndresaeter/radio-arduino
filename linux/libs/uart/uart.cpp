#include <epollevent.hpp>
#include <iomanip>
#include <sys/eventfd.h>
#include <thread>
#include <uart.hpp>
#include <unistd.h>

using namespace std::chrono_literals;

Uart::Uart(std::string name, int baudrate, control_modes modes)
    : m_mode(modes)
    , m_name(name)
    , m_baudrate(baudrate)
{
    open();
}

int Uart::getFd() { return (m_fd); }

void Uart::write(const std::vector<uint8_t>& data) const
{
    if (m_printDebug) {
        std::cout << "w: ";
        for (int i = 0; i < (data.size()); i++) {
            std::cout << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << static_cast<int>(data.at(i))
                      << " ";
        }
        std::cout << std::endl;
    }

    const uint8_t* p = data.data();
    if (::write(m_fd, &p[0], data.size()) == 0) {
        std::cout << "WARNING: not able write to serial port" << std::endl;
    }
}

std::vector<uint8_t> Uart::read()
{
    ssize_t bytesRead;
    char buf[READ_CHUNK_SIZE] = {};
    std::vector<uint8_t> data;

    // fill up vector data one chunck at a time
    while (bytesRead = ::read(m_fd, (char*)&buf, READ_CHUNK_SIZE)) {
        for (ssize_t i = 0; i < bytesRead; i++) {
            data.push_back(buf[i]);
        }
    }

    return data;
}

void Uart::execute()
{
    // called by EventProcess when data is received on the uart
    std::vector<uint8_t> data;
    data = read();
    std::string debug {};

    if (m_printDebug) {
        std::cout << "r: ";
        for (int i = 0; i < (data.size()); i++) {
            std::cout << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << static_cast<int>(data.at(i))
                      << " ";
            debug += data.at(i);
        }
        std::cout << std::endl;
    }

    if (m_extraSleep) {
        m_extraSleep = false;
        std::this_thread::sleep_for(10ms);
    }

    if (m_readhandler != nullptr) {
        try {
            m_readhandler(data); // m_parser.receive(data);
        }
        catch (std::exception e) {
            std::cout << "exception reading serial port: " << e.what() << std::endl;
        }
    }
}

void Uart::setExtraSleep() { m_extraSleep = true; }

void Uart::setReadHandler(ReadHandler readhandler) { m_readhandler = readhandler; }

void Uart::open()
{
    if (m_fd = ::open(m_name.c_str(), O_RDWR)) {
        EpollEvent::m_fd = m_fd;
        configure();
    }
    else {
        std::cout << "could not open serial port. exit" << std::endl;
        exit(1);
    }
}
void Uart::close()
{
    if (m_fd >= 0) {
        ::close(m_fd);
    }
}

void Uart::configure()
{
    fcntl(m_fd, F_SETFL, O_NONBLOCK);

    auto mode = m_mode;

    struct termios tty;
    memset(&tty, 0, sizeof tty);

    if (tcgetattr(m_fd, &tty) != 0) {
        std::cout << "ERROR: error from tcgetattr" << std::endl;
    }
    if (mode.parity) {
        tty.c_cflag |= PARENB;
    }
    else {
        tty.c_cflag &= ~PARENB; // no parity
    }

    if (mode.stop > 1) {
        tty.c_cflag |= CSTOPB;
    }
    else {
        tty.c_cflag &= ~CSTOPB; // one stop bit
    }

    if (mode.hangup_on_close) {
        // disable hang-up-on-close to avoid reset
        tty.c_cflag &= ~HUPCL;
    }

    switch (mode.bits) {
    case 5:
        tty.c_cflag |= CS5;
        break;
    case 6:
        tty.c_cflag |= CS6;
        break;
    case 7:
        tty.c_cflag |= CS7;
        break;
    default:
    case 8:
        tty.c_cflag |= CS8;
        break;
    }

    if (mode.hw_flow) {
        tty.c_cflag |= CRTSCTS;
    }
    else {
        tty.c_cflag &= ~CRTSCTS;
    }

    tty.c_cflag |= CREAD | CLOCAL; // read and ignore control lines

    // local flags
    tty.c_lflag &= ~ICANON;                  // turn off caninical mode
    tty.c_lflag &= ~(ECHO | ECHOE | ECHONL); // disable echo
    tty.c_lflag &= ~(ISIG);                  // disable intepretation of INTR,QUIT and SUSP

    // input flags
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);                                      // turn off sw flow control
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // disable special character handling

    // output flags
    tty.c_oflag &= ~OPOST; // do not interpret newlines
    tty.c_oflag &= ~ONLCR; // do not convert cr/lf etc
    // no waiting for data
    tty.c_cc[VTIME] = 0;
    tty.c_cc[VMIN] = 0;

    switch (m_baudrate) {
    case 1000000:
        cfsetispeed(&tty, B1000000);
        break;
    case 115200:
        cfsetispeed(&tty, B115200);
        break;
    case 57600:
        cfsetispeed(&tty, B57600);
        break;
    case 9600:
        cfsetispeed(&tty, B9600);
        break;
    default:
        printf("DEBUG: set speed unknown\n");
        cfsetispeed(&tty, m_baudrate);
        exit(1);
    }

    if (tcsetattr(m_fd, TCSANOW, &tty) != 0) {
        std::cout << "ERROR: error from tcsetattr" << std::endl;
    }
}

void Uart::printDebug(bool d) { m_printDebug = d; }
