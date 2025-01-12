#include "common.h"

char* InitShm() {
    std::cout << ALICE_MAGIC << " " << BOB_MAGIC << std::endl;
    int shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT|0666);
    if(shmid == -1) {
        std::cerr << "shmget error!" << std::endl;
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
    }
    std::cout << "shmid = " << shmid << std::endl;
    char* ret = (char*)shmat(shmid, NULL, 0);
    if(ret == (void*)-1) {
        std::cerr << "shmat error!" << std::endl;
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
    }
    auto index = (long long*)(((char*)ret) + INDEX_OFFSET);
    (*index) = 0; // init index
    auto magic = (std::atomic<std::uint64_t>*)(((char*)ret) + MAGIC_OFFSET);
    magic->store(BOB_MAGIC, std::memory_order_release);
    std::cout << "Bob init" << std::endl;
    return ret;
}

char* shm_ptr = InitShm();

void send(const Message *message)
{
    auto magic = GetMagicPtr();
    magic->store(ALICE_MAGIC, std::memory_order_release);
}

Message *recv()
{
    static long long expect = 1;
    auto index = GetIndexPtr();
    auto magic = GetMagicPtr();
    while(magic->load() != BOB_MAGIC || *index != expect) {
        continue;
    }
    expect++;
    return GetAliceDataPtr();
}

int main()
{
    Message *m2 = GetBobDataPtr();
    while (true)
    {
        Message *m1 = recv();
        assert(m1->checksum == crc32(m1));
        // memcpy(m2, m1, m1->size); // 拷贝m1至m2
        m2 = m1;
        m2->payload[0]++;         // 第一个字符加一
        m2->checksum = crc32(m2); // 更新校验和
        send(m2);
    }

    return 0;
}