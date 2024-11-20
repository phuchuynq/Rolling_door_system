# Hệ thống cửa cuốn thông minh

# Mô tả hoạt động của hệ thống
Hệ thống điều khiển cửa thông minh sử dụng ESP32, thẻ RFID và Blynk, giao diện Web để quản lý dữ liệu và điều khiển phần cứng. Khi thẻ từ được quét và có trong cơ sở dữ liệu, cửa sẽ mở nhờ động cơ bước quay. Khi động cơ quay đến vị trí đã định, công tắc hành trình trên sẽ dừng động cơ. Nếu thẻ từ được quét lại lần thứ hai, động cơ sẽ quay ngược lại để đóng cửa, và khi cửa đến vị trí đóng hoàn toàn, công tắc hành trình dưới sẽ dừng động cơ. Ngoài ra cũng có thể điều khiển thông qua 3 nút bấm (mở, đóng, dừng) động cơ. Cũng có thể điều khiển cửa thông qua app Blynk và Webserver 


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
- Lập trình **HTML, CSS, JavaScript** để khởi tạo giao diện hệ thống, mục đích lưu trữ cơ sở dữ liệu, điều khiển chức năng.
## Tiến Trình Chuyển Giao Mã Khi Người Dùng Sử Dụng Giao Diện

### 1. **Trang đăng nhập (loginPage)**
Khi người dùng truy cập vào hệ thống lần đầu tiên, trang đăng nhập sẽ yêu cầu nhập tên đăng nhập và mật khẩu.
- Nếu thông tin đăng nhập chính xác, hệ thống sẽ chuyển hướng người dùng tới trang chính (`homePage`).
- Nếu đăng nhập thất bại, hệ thống sẽ hiển thị thông báo lỗi.

### 2. **Trang chính (homePage)**
Sau khi đăng nhập thành công, người dùng sẽ được chuyển đến trang chính của hệ thống:
- **Kiểm tra trạng thái đăng nhập**: Nếu người dùng chưa đăng nhập, hệ thống sẽ tự động chuyển về trang đăng nhập.
- **Chức năng Đăng Xuất**: Người dùng có thể đăng xuất và quay lại trang đăng nhập.

### 3. **Trang quản lý thẻ RFID (dataPage)**
Sau khi đăng nhập thành công, người dùng có thể quản lý các thẻ RFID thông qua giao diện này:
- **Thêm Thẻ RFID**: Người dùng có thể thêm một thẻ mới vào hệ thống, với mã thẻ và tên thẻ.
- **Xóa Thẻ RFID**: Người dùng có thể xóa một thẻ đã đăng ký khỏi hệ thống.
- **Hiển Thị Danh Sách Thẻ**: Hệ thống sẽ tự động tải và hiển thị danh sách các thẻ đã được đăng ký.

### 4. **Trang cài đặt (setupPage)**
Trang này cho phép người dùng thực hiện các cài đặt liên quan đến hệ thống cửa và thẻ RFID:
- **Cài Đặt Hệ Thống**: Người dùng có thể thay đổi các cài đặt hệ thống nếu cần thiết.
- **Kiểm Tra Trạng Thái Đăng Nhập**: Trang này chỉ có thể truy cập nếu người dùng đã đăng nhập thành công.

### 5. **Tính năng quản lý tthẻ RFID**
- **Thêm Thẻ RFID**: Sau khi nhập mã thẻ và tên thẻ, hệ thống sẽ lưu trữ thẻ vào cơ sở dữ liệu.
- **Xóa Thẻ RFID**: Khi người dùng xác nhận xóa một thẻ, hệ thống sẽ loại bỏ thẻ đó khỏi danh sách.
- **Danh Sách Thẻ RFID**: Danh sách tất cả các thẻ RFID đã đăng ký sẽ được hiển thị dưới dạng bảng, và người dùng có thể chọn bất kỳ thẻ nào để sửa hoặc xóa.

### 6. **Quá trình thực hiện**
- **Đăng nhập**: Người dùng nhập tên đăng nhập và mật khẩu. Nếu đúng, chuyển sang trang chính, nếu sai sẽ thông báo lỗi.
- **Truy cập trang chính**: Trang này kiểm tra trạng thái đăng nhập và cung cấp các chức năng đăng xuất.
- **Quản lý thẻ RFID**: Sau khi vào trang quản lý thẻ, người dùng có thể thêm mới hoặc xóa thẻ và xem danh sách thẻ đã đăng ký.
- **Cài đặt hệ thống**: Trang cài đặt cung cấp các lựa chọn để cấu hình hệ thống và thẻ RFID.

## Hướng dẫn sử dụng Website

1. **Đăng nhập**: Truy cập vào trang đăng nhập, nhập tên đăng nhập và mật khẩu. Nếu đúng, bạn sẽ được chuyển đến trang chính.
2. **Trang chính**: Tại đây, bạn có thể chọn "Đăng xuất" để thoát khỏi hệ thống.
3. **Quản lý thẻ RFID**:
   - Nhập mã thẻ và tên thẻ trong các ô nhập liệu.
   - Nhấn nút "Thêm" để thêm thẻ mới vào hệ thống.
   - Nhấn nút "Xóa" để xóa thẻ đã chọn.
   - Danh sách thẻ sẽ được hiển thị dưới dạng bảng. Bạn có thể nhấp vào bất kỳ dòng nào để xem chi tiết thẻ.
4. **Trang cài đặt**: Cấu hình các thiết lập của hệ thống nếu cần thiết.

## Vì sao làm dự án này, tính khả thi của dự án
Sự phát triển của điện tử và lập trình, hiện nay đã có nhiều nơi trển khai nhiều mô hình IoT, điển hình là nhà thông minh, cửa thông minh,... thì cửa cuốn thông minh cũng là 1 trong số đó. Mô hình cửa cuốn thông minh có tính khả thi cao, cả về mặt kỹ thuật và ứng dụng thực tế. Các linh kiện cần thiết đều sẵn có với chi phí hợp lý. Với khả năng mở rộng và tính năng bảo mật, cửa cuốn thông minh không chỉ mang lại sự tiện lợi mà còn đảm bảo an toàn và kiểm soát cao, phù hợp với các yêu cầu hiện đại về tự động hóa và an ninh.

## Kinh nghiệm tích lũy từ dự án
Em đã học được cách tích hợp phần cứng và phần mềm để tạo ra một hệ thống hoàn chỉnh, từ lập trình C trên vi điều khiển ESP32 cho đến viết HTML, CSS, Javascript để khởi tạo 1 trang web có thể tương tác với phần cứng thông qua giao thức websocket. Thêm vào đó em cũng hiểu rõ hơn về việc quản lý thời gian xử lý và đảm bảo tính ổn định cho hệ thống, việc làm việc với các cảm biến và module khác nhau giúp em cải thiện kỹ năng xử lý và tích hợp hệ thống an toàn.

[Xem hình ảnh thực tế và video demo hệ thống](https://drive.google.com/drive/folders/1_SHG40nfEkMB5-iA8ZEfWTfBTKq0bDkS?dmr=1&ec=wgc-drive-globalnav-goto)





