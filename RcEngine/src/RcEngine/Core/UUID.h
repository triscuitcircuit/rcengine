//
// Created by Tristan Zippert on 10/14/21.
//
#ifndef RCENGINE_UUID_H
#define RCENGINE_UUID_H

namespace RcEngine{
    class UUID{
    public:
        UUID();
        UUID(uint64_t uuid);
        UUID(const UUID&) = default;

        operator uint64_t() const {return m_UUID;}
    private:
        uint64_t m_UUID;
    };
}
namespace std{
    template<>
    struct hash<RcEngine::UUID>{
        std::size_t operator() (const RcEngine::UUID& uuid)const{
            return hash<uint64_t>()((uint64_t)uuid);
        }
    };
}

#endif //RCENGINE_UID_H
