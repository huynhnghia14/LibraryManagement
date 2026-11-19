// Đo hiệu năng các CTDL: chạy bằng "make bench"
#include "HashTable.h"
#include "Reader.h"
#include "Utils.h"

#include <chrono>
#include <cstdio>
#include <cstring>
#include <string>

namespace {
    typedef std::chrono::high_resolution_clock Clock;

    double msSince(Clock::time_point start) {
        return std::chrono::duration<double, std::milli>(Clock::now() - start).count();
    }

    std::string makeCmnd(int i) {
        char buf[16];
        snprintf(buf, sizeof buf, "079%09u", (unsigned)i * 7919u % 1000000000u);
        return buf;
    }

    // Bảng băm vs tìm tuần tự trên mảng: n độc giả, q truy vấn (một nửa có, một nửa không có)
    void benchHashVsLinear(int n, int q) {
        HashTable<std::string, Reader> table;
        Reader* arr = new Reader[n];
        for (int i = 0; i < n; ++i) {
            Reader r;
            memset(&r, 0, sizeof r);
            Utils::copyStr(r.cmnd, makeCmnd(i), CMND_LEN);
            table.insert(r.cmnd, r);
            arr[i] = r;
        }

        int foundHash = 0, foundLinear = 0;
        Clock::time_point t0 = Clock::now();
        for (int k = 0; k < q; ++k) {
            if (table.find(makeCmnd(k % 2 == 0 ? k % n : n + k))) ++foundHash;
        }
        double hashMs = msSince(t0);

        t0 = Clock::now();
        for (int k = 0; k < q; ++k) {
            std::string key = makeCmnd(k % 2 == 0 ? k % n : n + k);
            for (int i = 0; i < n; ++i) {
                if (key == arr[i].cmnd) {
                    ++foundLinear;
                    break;
                }
            }
        }
        double linearMs = msSince(t0);

        printf("| %7d | %6d | %10.3f | %12.3f | %7.1fx | %5.2f | %5d |\n", n, q, hashMs, linearMs,
               hashMs > 0 ? linearMs / hashMs : 0.0, table.loadFactor(), table.longestChain());
        if (foundHash != foundLinear) printf("  !! ket qua khac nhau: %d vs %d\n", foundHash, foundLinear);
        delete[] arr;
    }
}

int main() {
    printf("== Bang bam (separate chaining) vs tim tuan tu - tim doc gia theo CMND ==\n");
    printf("|       n |      q |  Hash (ms) | Tuan tu (ms) | Nhanh hon | Tai  | Chuoi |\n");
    int sizes[] = {1000, 5000, 20000, 50000};
    for (int n : sizes) benchHashVsLinear(n, 2000);
    return 0;
}
