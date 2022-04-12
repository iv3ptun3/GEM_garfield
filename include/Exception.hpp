#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <string>

class Exception : public std::exception
{
    public:
    Exception(const std::string &message_) : std::exception(), message(message_){}

    virtual const char *what() const noexcept override
    {
        return message.c_str();
    }
    private:
    const std::string message;
};
#endif // EXCEPTION_HPP