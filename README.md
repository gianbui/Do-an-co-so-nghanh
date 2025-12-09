Tên đề tài: Thiết kế mô hình kiểm soát không khí trong nhà

1.Giới thiệu
- Dự án xây dựng một mô hình giám sát và cảnh báo chất lượng không khí trong nhà.
- Hệ thống sử dụng ESP32 kết hợp các cảm biến để đo nhiệt độ – độ ẩm – khí gas, điều khiển đèn LED và servo để mô phỏng quạt thông gió.
- Tất cả dữ liệu được hiển thị trên LCD 1602 và gửi lên Blynk App, cho phép người dùng theo dõi thông số từ xa.
2. Mục tiêu
- Thiết kế một hệ thống có thể duy trì nhiệt độ trong không gian kín. Có thể thiết lập nhiệt độ mà người sử dụng mong muốn, dựa theo yêu cầu đó hệ thống sẽ điều khiển các thiết bị để điều chỉnh nhiệt độ phòng đáp ứng mong muốn của người sử dụng.
3. Chức năng chính
- Đo nhiệt độ và độ ẩm bằng cảm biến DHT11
- Phát hiện khí gas bằng cảm biến MQ-2
- Hiển thị lên LCD 1602 (I2C)
- Điều khiển Servo (mở cửa hoặc quạt thông gió)
- Gửi dữ liệu lên Blynk và điều khiển từ xa
- Cảnh báo khi vượt ngưỡng (Gas hoặc nhiệt độ)
4.Thiết bị
- ESP32
- Cảm biến DHT11
- Cảm biến Gas MQ-2 
- LCD 1602 + module I2C
- Servo SG90
- Đèn LED cảnh báo
- Dây cắm, PCB, nguồn 5V
5. Cách chạy chương trình
- Cài Arduino IDE và driver cho ESP32.
- Cài các thư viện:
 DHT sensor library, LiquidCrystal_I2C, Blynk (New Blynk IoT)
- Mở 1.ino → chọn board: ESP32 Dev Module
- Tạo project trên Blynk → lấy Auth Token → dán vào code
- Kết nối ESP32 với USB → Upload
- Mở Serial Monitor để kiểm tra dữ liệu.
6. Phụ lục
[PHỤ LỤC HƯỚNG DẪN SỬ DỤNG SẢN PHẨM.docx](https://github.com/user-attachments/files/24050125/PH.L.C.H.NG.D.N.S.D.NG.S.N.PH.M.docx)
