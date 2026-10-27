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

## Checklist

| Tuần | Checklist |
|---|---|
| 1 | ☑ Repo Git ☑ SRS.pdf ☑ Cấu trúc thư mục |
| 2 | ☑ Design.pdf ☑ Sơ đồ lớp ☑ Định nghĩa struct |
| 3 | ☐ AVL ☐ BST ☐ Test quản lý sách |
| 4 | ☐ Hash Table ☐ Test quản lý độc giả |
| 5 | ☐ LinkedList ☐ Dynamic Array ☐ File I/O |
| 6 | ☐ Mượn sách ☐ Trả sách ☐ Tính phạt |
| 7 | ☐ Graph ☐ BFS/DFS ☐ Thống kê top 10 |
| 8 | ☐ CTDL mới (Trie) ☐ Tích hợp ☐ Demo |
| 9 | ☐ Unit test ☐ Bảng so sánh ☐ User manual |
| 10 | ☐ Báo cáo ☐ Video ☐ Slide ☐ Nộp + ký |
