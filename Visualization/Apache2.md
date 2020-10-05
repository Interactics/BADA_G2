
## Apache 설치   
sudo apt-get update   
sudo apt-get install apache2   
apache2 -v   
**버전확인이 되면 설치성공**   
   
   
   
   
   
## Apache 상태 확인과 시작/중지/재시작   
sudo service apache2 status   
->sudo service apache2 start    
->sudo service apache2 stop   
->sudo service apache2 restart   
   
**http://localhost 접속해서 apache 사이트 나오면 성공!**   
   
   
   
   
   
## Document Root 수정   
/etc/apache2/sites-available/000-default.con 파일의 DocumentRoot를 수정   
나는 '<생략>/ros2d/public' 으로 해뒀음   
   
   
   
   
   
   
## 권한 수정   
/etc/apache2/apache2.conf 파일 중반부의 코드를 수정한다.(denied->granted)   
   
    <Directory />
   
      AllowOverride None   
      
      Require all granted   
      
    </Directory>   
   
   
**위 과정을 완료하면 http://<ip주소>로 접속하면 index.html 파일의 웹페이지가 나와야함**   
   
   
   
   
   
## 네트워크 연결 후 모바일기기에서 웹사이트 확인하기   
   
1. 휴대폰 핫스팟으로 노트북 네트워크 연결    
2. 네트워크 프로필을 '개인'으로 변경   
3. 모바일 기기에서 http://<ip주소> 로 접속   
   
   
여기까지 성공하면 카카오톡에 도메인등록을 위해 지원이한테 연락해서 자신의 ip주소를 알려준다.   
   
**위 과정에서 수많은 에러를 만날수있음**




