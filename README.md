# Hệ thống cửa cuốn thông minh

# Mô tả hoạt động của hệ thống
Hệ thống điều khiển cửa thông minh sử dụng ESP32, RFID, và Blynk để quản lý trạng thái cửa, kiểm soát truy cập và cấu hình qua giao diện web. Khi thẻ từ được quét hợp lệ, UID thẻ có trong cơ sở dữ liệu thì động cơ bước hoạt động quay làm mở cửa. Khi động cơ quay chạm đến công tắc hành trình trên thì động cơ sẽ dừng lại, tương tự khi quét thẻ lại lần 2 thì động cơ quay ngược chiều mở cửa làm đóng cửa, tương tự khi chạm đến công tắc hành trình dưới thì động cơ dừng lại và cửa đóng hoàn toàn.


## Vai trò trong dự án
Lập trình code C, triển khai phần cứng, thiết kế giao diện hệ thống trên app Blynk, tạo giao diện web để quản lý cơ sở dữ liệu và điều khiển động cơ.

## Chi tiết về phần cứng và phần mềm

### Phần cứng
- Bo mạch **ESP32** với 30 chân để điều khiển các thiết bị trong hệ thống.
- **Module RFID MFRC522** để nhận diện thẻ từ.
- **Động cơ bước(step motor)** để điều khiển mở/đóng cửa.
- **Driver TB6600** để điều khiển động cơ bước.
- **2 công tắc hành trình** để giới hạn vị trí động cơ.
- **3 button** để điều khiển cửa 1 cách thủ công (mở, đóng, dừng).
- **Biến trở 10kOhm** để giảm nguồn cấp lên động cơ từ đó quyết định được động cơ quay nhanh hay chậm.
- Dữ liệu thẻ được lưu trữ trên **Web Server**, đông thời có thể điều khiển cửa và thêm bớt dữ liệu thẻ UID.

### Phần mềm
- Lập trình bằng **Arduino IDE** để điều khiển ESP32.
- Viết **Google Apps Script** để tương tác với Google Sheets.  
- Code Arduino được thiết kế để xử lý quét thẻ, điều khiển động cơ servo, và giám sát thời gian vào/ra theo yêu cầu dự án.

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
