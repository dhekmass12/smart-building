# smart-building
Current feature: temperature control and fire detection system

Sistem ini dirancang untuk menurunkan suhu panas dan juga mendeteksi adanya kebakaran dalam suatu gedung.

Team's name: **Pintu Surga**.

Members:
1. Dimas Tri Kurniawan (2006530261)
2. Adnan Mukhtar (2006485245)
3. Bintang Hari Pratama (2006462696)
4. Ahmad Ghozi Fidnillah (2006473932)

Created & written by: **Dimas Tri Kurniawan**

Alat yang digunakan:
- 5 buah led merah
- 5 buah sensor mq-2
- 5 buah sensor dht22
- 5 buah transistor
- 5 buah kipas
- 5 buah buzzer

Prasyarat:
1. FreeRTOS
2. Install library: Adafruit Unified Sensor, Buzzer, DHT sensor library, FreeRTOS

Cara kerja:
1. Saat sistem dinyalakan, sistem akan melakukan kalibrasi semua sensor mq-2 selama ± 1 menit guna mendapatkan Ro dari setiap lantai.
2. Sistem akan mengambil informasi mengenai suhu dan asap di setiap lantai secara real-time dan juga contiguous sekaligus menampilkannya melalui serial.
3. Terdapat 2 state utama yang nantinya akan dipilih oleh state sesuai informasi yang didapat dari step 2:
- State deteksi suhu panas: kondisi ini terjadi apabila sistem mendeteksi suhu ruangan >= 35°C. Sistem akan menghidupkan kipas untuk menurunkan suhu ruangan.
- State deteksi kebakaran: kondisi ini terjadi apabila sistem mendeteksi kandungan asap dalam ruangan >= 50ppm. Sistem akan menyalakan buzzer dan lampu led secara kelap-kelip.

Hal yang harus diperhatikan:
1. Isi pin dan buat task untuk lantai 3, 2, 1.
2. Tidak boleh merubah format output. Cukup ubah data yang diperlukan.
3. Nama folder dan file .ino harus sama. Hati-hati mengubah nama file yang ada dependensinya.
4. Jaga jarak saat mengetes sensor. Khusus untuk mq-2, jangan sampai kepanasan karena mudah rusak.
