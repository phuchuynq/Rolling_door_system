# Smart Door Control System

Hệ thống điều khiển cửa thông minh sử dụng ESP32, RFID, và Blynk để quản lý trạng thái cửa, kiểm soát truy cập và cấu hình qua giao diện web hoặc ứng dụng.

## Mục lục
1. [Giới thiệu](#giới-thiệu)
2. [Tính năng](#tính-năng)
3. [Yêu cầu phần cứng](#yêu-cầu-phần-cứng)
4. [Cài đặt và sử dụng](#cài-đặt-và-sử-dụng)
5. [Hướng dẫn cấu hình](#hướng-dẫn-cấu-hình)
6. [Hình ảnh minh họa](#hình-ảnh-minh-họa)
7. [Tác giả](#tác-giả)

---

## Giới thiệu

Hệ thống này cung cấp giải pháp điều khiển cửa tự động với khả năng nhận diện RFID, điều khiển động cơ bước, và tích hợp ứng dụng di động qua Blynk. Hệ thống hỗ trợ các tính năng như cấu hình qua WiFi, quản lý dữ liệu thẻ RFID, và theo dõi trạng thái cửa trong thời gian thực.

---

## Tính năng

- **Quản lý truy cập bằng RFID:** Nhận diện thẻ hợp lệ để mở/đóng cửa.
- **Điều khiển động cơ bước:** Hỗ trợ đóng/mở cửa với công tắc hành trình để xác định trạng thái cuối.
- **Giao diện web:** Cấu hình WiFi, quản lý tài khoản và dữ liệu thẻ.
- **Tích hợp Blynk:** Theo dõi trạng thái cửa và điều khiển từ xa qua ứng dụng di động.
- **Thông báo qua WebSocket:** Gửi thông tin trạng thái hoặc dữ liệu truy cập.
- **Cấu hình chế độ WiFi:** Hỗ trợ AP Mode (phát WiFi) và STA Mode (kết nối WiFi).

---

## Yêu cầu phần cứng

- **Vi điều khiển:** ESP32 (30 chân).
- **Đầu đọc RFID:** MFRC522.
- **Động cơ bước:** Loại sử dụng với driver (ví dụ: A4988, DRV8825).
- **Driver động cơ:** Driver phù hợp với động cơ bước.
- **Buzzer:** Tích hợp thông báo âm thanh.
- **Các nút bấm:** Mở, đóng, dừng.
- **Công tắc hành trình:** Giới hạn trạng thái mở/đóng cửa.
- **Nguồn cấp:** 5V/3.3V tùy theo phần cứng.

---

## Cài đặt và sử dụng

1. **Kết nối phần cứng:** 
   - Kết nối các thành phần như mô tả trong sơ đồ mạch (chưa có sẵn ở đây, cần cập nhật hình minh họa).
   - Lưu ý kiểm tra các chân được định nghĩa trong code (`#define`).

2. **Tải code lên ESP32:**
   - Sử dụng Arduino IDE với các thư viện cần thiết:
     - `MFRC522`
     - `AccelStepper`
     - `BlynkSimpleEsp32`
     - `ArduinoJson`
   - Tải code lên ESP32.

3. **Cấu hình hệ thống:**
   - Kết nối với WiFi phát ra từ ESP32 (AP Mode).
   - Truy cập giao diện web tại `192.168.4.1`.
   - Cấu hình WiFi, tài khoản, và các thông tin khác.

4. **Tích hợp Blynk:**
   - Sử dụng mã Blynk được nhập trong giao diện cấu hình.
   - Theo dõi trạng thái cửa và điều khiển qua ứng dụng Blynk.

---

## Hướng dẫn cấu hình

1. **Reset về mặc định:**
   - Nhấn và giữ nút cấu hình (`btSetup`) trong 15 giây để reset hệ thống.
2. **Cấu hình tài khoản:**
   - Sử dụng giao diện web để thay đổi tên đăng nhập và mật khẩu.
3. **Thêm/xóa dữ liệu thẻ:**
   - Thêm hoặc xóa thẻ RFID trực tiếp qua giao diện web.

---

## Hình ảnh minh họa

*(Thêm các hình ảnh về hệ thống, sơ đồ mạch, giao diện web, hoặc ứng dụng Blynk tại đây.)*

---

## Tác giả

- **Tên:** *(Thêm tên hoặc nickname của bạn)*
- **Liên hệ:** *(Email hoặc liên kết GitHub cá nhân)*
- **Ghi chú:** Vui lòng liên hệ nếu có bất kỳ thắc mắc nào liên quan đến dự án.

---

## Ghi chú bổ sung

Dự án này được phát triển với mục đích cá nhân và học tập. Vui lòng trích dẫn tác giả nếu sử dụng hoặc chia sẻ mã nguồn.
