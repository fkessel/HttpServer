#pragma once

#include <string>

#include "Message.hpp"

namespace http {

    enum class Method {
        GET_M,
        POST_M,
        HEAD_M,
        PUT_M,
        DELETE_M,
        CONNECT_M,
        OPTIONS_M,
        TRACE_M,
        VOID_M,
    };

    typedef std::string Target;

    class RequestLine {

    private:
        Target target_;
        Method method_;

    public:

        RequestLine() {}

        Target getTarget() const { return target_; }

        Method getMethod() const { return method_; }

        void setTarget(const std::string& target)
        {
            target_ = target;
        }

        void setMethod(const std::string& method)
        {
            determineMethod(method);
        }

    private:
        void determineMethod(const std::string& method)
        {
            if (method.empty())
            {
                method_ = Method::VOID_M;
                return;
            }

            std::string temp = Tools::stringToLower(method);

            // TODO: send a message when there is a method which is not supported

            if (temp == "get") { method_ = Method::GET_M; }
            else if (temp == "post") { method_ = Method::POST_M; }
            else {method_ = Method::VOID_M; }
        }
    };
}