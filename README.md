

![](https://user-images.githubusercontent.com/100114914/169705468-3cc02aa4-c64b-4a20-aa6b-9e17124a72ff.png)

> # 																	**2048 Project**
>
> Ma Thanh Thiện 21020794
>
> K66-CD CN1-UET
>
> mathanhthien@gmail.com
>
> Link Demo:[2048 SDL - YouTube](https://www.youtube.com/watch?v=yGhtVYZeI4M)

## **GIỚI THIỆU CHUNG:**

2048 được lấy cảm hứng từ [2048 (play2048.co)](https://play2048.co/) từ hình ảnh cho tới cơ chế.

Game được viết bằng ngôn ngữ C++ và sử dụng thư viện SDL2

##### LUẬT CHƠI:

- **Người chơi sẽ được cấp cho một bảng kích cỡ 4x4 với 2 ô số mở đầu (mặc định là 2 số 2) những ô có cùng giá trị sẽ sát nhập vào nhau và tạo ra một ô có giá trị gấp đôi. Nhiệm vụ của người chơi là sát nhập các ô một cách khéo léo để tạo ra ô chiến thắng 2048. Bằng cách**
  - Nhấn UP để dồn các ô lên phía trên

  - Nhấn DOWN để dồn các ô xuống phía dưới

  - Nhấn LEFT để dồn các ô sang bên trái

  - Nhấn RIGHT để dồn các ô sang bên phải

  - Nhấn M để bật/tắt nhạc

- **Mỗi khi 2 ô cùng giá trị và được sát nhập điểm của người chơi sẽ được công thêm bằng đúng giá trị của ô mới được tạo ra từ việc sát nhập**

## CÁCH CÀI ĐẶT:

- Truy cập [thanhthien1305/2048-project (github.com)](https://github.com/thanhthien1305/2048-project)
- Tải xuống các file và bật 2048 project.sln
- Ctril F5 hoặc khởi chạy code



- ## MÔ TẢ CHỨC NĂNG

  - **Có nút Newgame để bắt đầu/ chơi lại ván game:**
  - **Có hệ thống âm thanh sinh động:**
  - **Lưu điểm cao  mỗi lần chơi**
  - **Có nút M để tắt bật âm** 
  - **Có bảng thông báo mỗi khi thắng hoặc thua**
  - **Có bảng Score cập nhập điểm liên tục và Best cập nhập điểm cao kể cả khi tắt chương trình**

  ## CÁC KĨ THUẬT LẬP TRÌNH ĐƯỢC SỬ DỤNG ##

  - Con trỏ
  - Class/Struct
  - Bắt sự kiện bàn phím, chuột
  - Stringstream
  - Cắm cờ
  - Sinh số ngẫu nhiên 
  - Đọc, ghi file
  - Vector 
  - Âm thanh trong SDL
  - Tải hình ảnh, clip trong máy lên màn hình của SDL
  - Tạo hình ảnh từ một text của SDL
  - Giải phóng bộ nhớ
  - String

  ## KẾT LUẬN

  Hướng phát triển: Em sẽ cố gắng phát triển phần chuyển động của các ô sao cho phù hợp logic sát nhập (vì phần đó quá rắc rối nên em chưa thể tối ưu code phần chuyển động và vì lúc quãng chuyển động + sát nhập quá ngắn... nên em đã bỏ đoạn code lỗi đó), tạo background scolling cho người chơi có trải nghiệm game đã mắt hơn. Tạo thêm tính năng thay đổi nhạc theo số lượng ô còn sót lại (số ô càng ít nhạc càng dồn dập -> tạo kịch tính cho người chơi). Tạo thêm lựa chọn chế độ bảng 6x6, 8x8 hoặc khó hơn là việc bảng tự mở rộng kích cỡ trong quá trình chơi mà không cần chọn chế độ từ đầu. Còn về menu, bởi vì từ đầu em đã tôn trọng và làm theo y như nguyên tác nên em nghĩ ta không cần thêm. Còn về điểm yếu nhất là chia file, em sẽ cố gắng học hỏi thêm sao cho việc chia không bị lỗi.

  

  Tự mình học ngôn ngữ và lập trình ra một game cho riêng bản thân, giúp em học được khả năng tự học, đọc hiểu tiếng anh. Sau khi hoàn thành game, kiến thức về tổ chức, sắp xếp các đối tượng của em đã được cải thiện rõ rệt. 

  

  **Lời cuối cùng, xin cảm ơn các thầy:**

  - Thầy Trần Quốc Long
  - Thầy Nguyễn Việt Anh
  - Thầy Nguyễn Thành Công

  Vì đã giúp đỡ, chỉ dạy, hướng dẫn em hoàn thành dự án game này.
