#include "Order.h"

namespace state
{
    const std::string& Order::getOriginProvinceId() const
    {
        return originProvinceId;
    }
    void Order::setOriginProvinceId(const std::string& originProvinceId)
    {
        this->originProvinceId = originProvinceId;
    }
    const std::string& Order::getDestinationProvinceId() const
    {
        return destinationProvinceId;
    }
    void Order::setDestinationProvinceId(const std::string& destinationProvinceId)
    {
        this->destinationProvinceId = destinationProvinceId;
    }
    int Order::getDuration() const
    {
        return duration;
    }
    void Order::setDuration(int duration)
    {
        this->duration = duration;
    }
    int Order::getElapsed() const
    {
        return elapsed;
    }
    void Order::setElapsed(int elapsed)
    {
        this->elapsed = elapsed;
    }
}