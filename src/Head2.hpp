#include <functional>
#include <memory>
#include <mutex>
#include <vector>
#include <thread>
#include "Head.hpp"

class Head2 : public Head {
    public:
        Head2(int id);
        virtual void execute(std::function<void()> action) override;
        std::shared_ptr<std::mutex> getRight();
        void setLeft(std::weak_ptr<std::mutex> setter);
        static int quantity;

    protected:
        int m_id;
        std::shared_ptr<std::mutex> m_mutexRight;
        std::weak_ptr<std::mutex> m_mutexLeft;
};