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
## Tiến Trình Chuyển Giao Mã Khi Người Dùng Sử Dụng Giao Diện

### 1. **Trang Đăng Nhập (loginPage)**
Khi người dùng truy cập vào hệ thống lần đầu tiên, trang đăng nhập sẽ yêu cầu nhập tên đăng nhập và mật khẩu.
- Nếu thông tin đăng nhập chính xác, hệ thống sẽ chuyển hướng người dùng tới trang chính (`homePage`).
- Nếu đăng nhập thất bại, hệ thống sẽ hiển thị thông báo lỗi.

### 2. **Trang Chính (homePage)**
Sau khi đăng nhập thành công, người dùng sẽ được chuyển đến trang chính của hệ thống:
- **Kiểm tra trạng thái đăng nhập**: Nếu người dùng chưa đăng nhập, hệ thống sẽ tự động chuyển về trang đăng nhập.
- **Chức năng Đăng Xuất**: Người dùng có thể đăng xuất và quay lại trang đăng nhập.

### 3. **Trang Quản Lý Thẻ RFID (dataPage)**
Sau khi đăng nhập thành công, người dùng có thể quản lý các thẻ RFID thông qua giao diện này:
- **Thêm Thẻ RFID**: Người dùng có thể thêm một thẻ mới vào hệ thống, với mã thẻ và tên thẻ.
- **Xóa Thẻ RFID**: Người dùng có thể xóa một thẻ đã đăng ký khỏi hệ thống.
- **Hiển Thị Danh Sách Thẻ**: Hệ thống sẽ tự động tải và hiển thị danh sách các thẻ đã được đăng ký.

### 4. **Trang Cài Đặt (setupPage)**
Trang này cho phép người dùng thực hiện các cài đặt liên quan đến hệ thống cửa và thẻ RFID:
- **Cài Đặt Hệ Thống**: Người dùng có thể thay đổi các cài đặt hệ thống nếu cần thiết.
- **Kiểm Tra Trạng Thái Đăng Nhập**: Trang này chỉ có thể truy cập nếu người dùng đã đăng nhập thành công.

### 5. **Tính Năng Quản Lý Thẻ RFID**
- **Thêm Thẻ RFID**: Sau khi nhập mã thẻ và tên thẻ, hệ thống sẽ lưu trữ thẻ vào cơ sở dữ liệu.
- **Xóa Thẻ RFID**: Khi người dùng xác nhận xóa một thẻ, hệ thống sẽ loại bỏ thẻ đó khỏi danh sách.
- **Danh Sách Thẻ RFID**: Danh sách tất cả các thẻ RFID đã đăng ký sẽ được hiển thị dưới dạng bảng, và người dùng có thể chọn bất kỳ thẻ nào để sửa hoặc xóa.

### 6. **Quá Trình Thực Hiện**
- **Đăng Nhập**: Người dùng nhập tên đăng nhập và mật khẩu. Nếu đúng, chuyển sang trang chính, nếu sai sẽ thông báo lỗi.
- **Truy Cập Trang Chính**: Trang này kiểm tra trạng thái đăng nhập và cung cấp các chức năng đăng xuất.
- **Quản Lý Thẻ RFID**: Sau khi vào trang quản lý thẻ, người dùng có thể thêm mới hoặc xóa thẻ và xem danh sách thẻ đã đăng ký.
- **Cài Đặt Hệ Thống**: Trang cài đặt cung cấp các lựa chọn để cấu hình hệ thống và thẻ RFID.

## Hướng Dẫn Sử Dụng

1. **Đăng Nhập**: Truy cập vào trang đăng nhập, nhập tên đăng nhập và mật khẩu. Nếu đúng, bạn sẽ được chuyển đến trang chính.
2. **Trang Chính**: Tại đây, bạn có thể chọn "Đăng xuất" để thoát khỏi hệ thống.
3. **Quản Lý Thẻ RFID**:
   - Nhập mã thẻ và tên thẻ trong các ô nhập liệu.
   - Nhấn nút "Thêm" để thêm thẻ mới vào hệ thống.
   - Nhấn nút "Xóa" để xóa thẻ đã chọn.
   - Danh sách thẻ sẽ được hiển thị dưới dạng bảng. Bạn có thể nhấp vào bất kỳ dòng nào để xem chi tiết thẻ.
4. **Trang Cài Đặt**: Cấu hình các thiết lập của hệ thống nếu cần thiết.







