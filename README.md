# LibraryManagement – Chương trình Quản lý Thư viện (C++)

Đồ án Tin học – Học kỳ I (2026–2027) – Lớp DH24_TH04 – GVHD: Lương An Vinh

Sinh viên: Trọng Nghĩa – MSSV: DH52400659

Chương trình quản lý thư viện viết bằng **C++11**, giao diện console, lưu trữ bằng
**file nhị phân** (không dùng CSDL), áp dụng các cấu trúc dữ liệu & giải thuật:

| CTDL / Giải thuật | Áp dụng cho chức năng |
|---|---|
| Cây AVL | Quản lý sách theo mã ISBN |
| Cây BST | In danh mục sách theo tên (in-order) |
| Bảng băm (separate chaining) | Quản lý độc giả theo CMND/CCCD |
| Danh sách liên kết đơn | Danh sách phiếu mượn |
| Mảng động | Lịch sử giao dịch |
| Đồ thị (danh sách kề) + BFS/DFS | Gợi ý sách cùng thể loại / liên quan |
| **Cây Trie** (CTDL tự tìm hiểu) | Tìm sách theo tiền tố tên (autocomplete) |
| Đọc / ghi file nhị phân | Lưu trữ toàn bộ dữ liệu |

## Cấu trúc thư mục

```
LibraryManagement/
├── include/      # header files (.h)
├── src/          # source files (.cpp)
├── data/         # file dữ liệu mẫu (.dat, .txt) và backup/
├── docs/         # báo cáo (SRS.pdf, Design.pdf, ...), hình ảnh sơ đồ (images/)
├── tests/        # test cases, benchmark
├── Makefile
└── README.md
```

## Biên dịch & chạy

```bash
make            # biên dịch -> bin/library
make run        # chạy chương trình
make test       # biên dịch và chạy các test trong tests/
make clean
```

Yêu cầu: g++ hỗ trợ C++11 (hoặc Dev-C++, Code::Blocks, Visual Studio).

## Tài liệu

- `docs/SRS.pdf` (+ `SRS.docx`) – Đặc tả yêu cầu phần mềm (Tuần 1)
- `docs/Design.pdf` (+ `Design.docx`) – Thiết kế CTDL, sơ đồ lớp/module, menu & luồng chương trình (Tuần 2)

## Tiến độ

| Tuần | Nội dung | Trạng thái |
|---|---|---|
| 1 | SRS, tạo repo, cấu trúc thư mục | ✅ |
| 2 | Design.pdf (sơ đồ lớp, struct, menu) | ✅ |
| 3 | AVL + BST quản lý sách | ⬜ |
| 4 | Hash Table quản lý độc giả | ⬜ |
| 5 | Linked List, Dynamic Array, File I/O | ⬜ |
| 6 | Mượn – trả, tính phạt | ⬜ |
| 7 | Graph, BFS/DFS, thống kê top 10 | ⬜ |
| 8 | Trie – autocomplete | ⬜ |
| 9 | Unit test, so sánh hiệu năng, user manual | ⬜ |
| 10 | Báo cáo, video, slide | ⬜ |
