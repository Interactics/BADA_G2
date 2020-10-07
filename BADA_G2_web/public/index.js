var data;
var time;
var sig_name;
var viewtime;
var PrintHistory= " ... ";

var hidx=0;
const FRAMES_PER_SECOND = 10;  // Valid values are 60,30,20,15,10...
const FRAME_MIN_TIME = (1000 / 60) * (60 / FRAMES_PER_SECOND) - (1000 / 60) * 0.5;
var lastFrameTime = 0;  // the last frame time

var canvas;
var ctx;
var start;
var oldTimeStamp;
var x;
var y;
var theta;
var str;
var data;
var time;
var sig_name;
var viewtime;
var PrintHistory = "Connection";

const today = new Date();
today.setTime(0);

const KAKAO_REST_API_TOKEN="KAKAO_REST_API_TOKEN";
const KAKAO_TOKEN="KAKAO_TOKEN";

Kakao.init(KAKAO_TOKEN);   // 사용할 앱의 JavaScript 키를 설정
// init();
/*  Kakao.Auth.createLoginButton({
    container: '#kakao-login-btn',
    size : 'small',
    success: function (authObj) {
      alert(JSON.stringify(authObj));
    },
    fail: function (err) {
      alert(JSON.stringify(err));
    }
  });*/

function setAuth() {
  Kakao.Auth.login({
    scope: 'talk_message,friends',
    success: function (response) {
      console.log(response);
    },
    fail: function (error) {
      console.log(error);
    }
  });
}
setAuth();
// Kakao.init(KAKAO_REST_API_TOKEN);   // 사용할 앱의 JavaScript 키를 설정

function shareKakaotalk(sig_name)
{
    Kakao.API.request({
      url: '/v2/api/talk/memo/default/send',
      data: {
        template_object: {
          object_type: 'text',
          text: sig_name+"가 발생한 것 같아요!",
          link: {
              web_url: 'http://192.168.0.193',
              mobile_web_url: 'http://192.168.0.193',
            },
            button_title : "BADA에서 확인하기"
        },
      },
      success: function(response) {
        console.log(response);
      },
      fail: function(error) {
        console.log(error);
      },
    });
  }

  // function shareKakaotalk(sig_name)
  // {
  //   Kakao.API.request({
  //     url: '/v2/api/talk/memo/default/send',
  //     data: {
  //       template_object: {
  //         object_type: 'text',
  //         text: sig_name,
  //         link: {
  //             web_url: 'http://192.168.137.1',
  //             mobile_web_url: 'http://192.168.137.1',
  //           },
  //       },
  //     },
  //     success: function(response) {
  //       console.log(response);
  //     },
  //     fail: function(error) {
  //       console.log(error);
  //     },
  //   });
  
  // }


  function shareauthKakaotalk()
  {
    Kakao.API.request({
      url: '/v2/api/talk/memo/default/send',
      data: {
        template_object: {
          object_type: 'text',
          text: "BADA 인증이 완료되었습니다",
          link: {
              web_url: 'http://192.168.0.193',
              mobile_web_url: 'http://192.168.0.193',
            },
            button_title : "BADA에서 확인하기"

        },
      },
      success: function(response) {
        console.log(response);
      },
      fail: function(error) {
        console.log(error);
      },
    });
  }
  shareauthKakaotalk();


  function printNow() {
    const today = new Date();
  
    // getDay: 해당 요일(0 ~ 6)를 나타내는 정수를 반환한다.
  
    const year = today.getFullYear();
    const month = today.getMonth() + 1;
    const date = today.getDate();
    let hour = today.getHours();
    let minute = today.getMinutes();
    let second = today.getSeconds();
    const ampm = hour >= 12 ? 'PM' : 'AM';
  
    // 12시간제로 변경
    hour %= 12;
    hour = hour || 12; // 0 => 12
  
    // 10미만인 분과 초를 2자리로 변경
    minute = minute < 10 ? '0' + minute : minute;
    second = second < 10 ? '0' + second : second;
  
    var now = `${year}.${month}.${date}    ${ampm} ${hour}:${minute}:${second} `;
    return now;
  };


  function printClock() {

    var clock = document.getElementById("date_and_time"); 
    var currentDate = new Date();
    var calendar = currentDate.getFullYear() + "-" + (currentDate.getMonth()+1) + "-" + currentDate.getDate() // 현재 날짜
    var AMPM = '오전';
    var currentHours = addZeros(currentDate.getHours(),2); 
    var currentMinute = addZeros(currentDate.getMinutes() ,2);
    var currentSeconds =  addZeros(currentDate.getSeconds(),2);
    
    if(currentHours >= 12){ 
    	AMPM = '오후';
    	currentHours = addZeros(currentHours - 12,2);
    }
    
    clock.innerHTML = currentHours+":"+currentMinute+":"+currentSeconds +" <span style='font-size:50px;'>"+ amPm+"</span>";
    setTimeout("printClock()",1000);         




    function addZeros(num, digit) {
        var zero = '';
        num = num.toString();
        if (num.length < digit) {
          for (i = 0; i < digit - num.length; i++) {
            zero += '0';
          }
        }
        return zero + num;
    }
  

};


function Queue(){

    this.dataStore = [];
    this.enqueue = enqueue;
    this.dequeue = dequeue;
    this.search=search;
    this.toString=toString;
}

function enqueue(element)
{
    this.dataStore.push(element);
}

function dequeue()
{   
    return this.dataStore.shift();
}


var cnt=0;
function search(){
    
    for(var i=0; i<this.dataStore.length;i++)
    {
      if((this.dataStore[i]-time)>1800000) //30분 이상이면 반복문탈출
      {
        break;
      }
      else{
        cnt++;
        console.log("Water events occur" + cnt + "times");
      }
    }
    if(cnt>=3)
    { console.log("Water event exceed 3 times. Send Message");
      return true;
    }
    else{
      return false;
    }
  }
  
  function toString() {
    var retStr = "";
    for (var i = this.dataStore.length-1;i >=0; i-- )    {
        retStr +="  "+ this.dataStore[i]+"\n";
    }
    retStr = retStr.replace(/(?:\r\n|\r|\n)/g, '<br />');
    return retStr;
}



var water= new Queue();
var h_element;
var h = new Queue();



  var ros = new ROSLIB.Ros();

  // If there is an error on the backend, an 'error' emit will be emitted.
  ros.on('error', function (error) {
    console.log(error);
  });

  // Find out exactly when we made a connection.
  ros.on('connection', function () {
    console.log('Connection made!');
  });

  ros.on('close', function () {
    console.log('Connection closed.');
  });

  // Create a connection to the rosbridge WebSocket server.
  ros.connect('ws://localhost:9090');

  // Like when publishing a topic, we first create a Topic object with details of the topic's name
  // and message type. Note that we can call publish or subscribe on the same topic object.
  var listener = new ROSLIB.Topic({
    ros: ros,
    name: '/listener',
    messageType: 'std_msgs/String'
  });

  // Then we add a callback to be called every time a message is published on this topic.
  listener.subscribe(function (message) {
    // console.log('Received message on ' + listener.name + ': ' + message.data);

    // If desired, we can unsubscribe from the topic as well.
    listener.unsubscribe();
  });

  

  
  var hsignal = new ROSLIB.Topic({
    ros : ros,
    name : '/signal',
    //name : '/bada_audio/signal',
    messageType : 'std_msgs/String'
  });
  const dic1={'Cry':'아기 우는 소리', 'Alarm':'화재 경보', 'Door':'노크', 'Boiling':'물 끓는 소리', 'Silence':'조용한', 'Water':'물소리', 'Bell':'초인종 소리'};

  hsignal.subscribe(function(m){

    sig_name=dic1[m.data];
    console.log("NOW SIGNAL : "+sig_name);


    time=today.getTime();
    viewtime=printNow();

    if(h.dataStore.length>=17)
    {
      h.dequeue();
    }
    
    if(sig_name==dic1['Water'])
    {
      //먼저 검색해 
      if(water.search())
      { 
        //총 3번 이상 발생했다면
        for(var i=1; i<=3; i++)
        {
          water.dequeue();
        }
        cnt=0;
        shareKakaotalk(sig_name);
        h.enqueue([sig_name, viewtime]);
        hidx=hidx+1;     
        PrintHistory=h.toString();
      }
      else
      {
       //발생한적없다면
       water.enqueue(time);
      }
    }
    else if(sig_name!=dic1["Silence"])
    {
        shareKakaotalk(sig_name);
        h.enqueue([sig_name, viewtime]);
        hidx=hidx+1;       
        PrintHistory=h.toString();
    }
    document.getElementById("history").innerHTML=PrintHistory;

  });

  //ALARM Section
  
  // Connecting to ROS
  
  var ros = new ROSLIB.Ros();
  
  // If there is an error on the backend, an 'error' emit will be emitted.
  ros.on('error', function (error) {
      console.log(error);
  });
  
  // Find out exactly when we made a connection.
  ros.on('connection', function () {
      console.log('Connection made!');
      init();
  });
  
  ros.on('close', function () {
      console.log('Connection closed.');
  });
  
  // Create a connection to the rosbridge WebSocket server.
  ros.connect('ws://localhost:9090');
  
  // Like when publishing a topic, we first create a Topic object with details of the topic's name
  // and message type. Note that we can call publish or subscribe on the same topic object.
  var listener = new ROSLIB.Topic({
      ros: ros,
      name: '/listener',
      messageType: 'std_msgs/String'
  });
  
  // Then we add a callback to be called every time a message is published on this topic.
  listener.subscribe(function (message) {
      // console.log('Received message on ' + listener.name + ': ' + message.data);
  
      // If desired, we can unsubscribe from the topic as well.
      listener.unsubscribe();
  });
  
  
  var ros = new ROSLIB.Ros({
      url: 'ws://localhost:9090'
  });
  
  
  var signal = new ROSLIB.Topic({
      ros : ros,
      name : '/signal',
     // name : '/bada_audio/signal',
      messageType : 'std_msgs/String'
    });
    // const dic1={'Speech':'말하는 소리', 'Alarm':'화재 경보', 'Door':'노크', 'Television':'티비 소리', 'Silence':'조용해요', 'Water':'물소리', 'Music':'휴대폰 벨소리'};
  
    signal.subscribe(function(m){
      sig_name=dic1[m.data];
      document.getElementById("signal").innerHTML = sig_name;
    });


//from index.js

//KAKAO_TOKEN="adwnM5UYyKBlm7Pg-9OC9BjoDwdUOUo8dCY9DgopyV4AAAFyI3fYdQ";
// Kakao.init("b886eede39b9d47bc9d3cb6e91483799");   // 사용할 앱의 JavaScript 키를 설정

 
  // 카카오 로그인 버튼을 생성합니다. 
 

    /*Kakao.Auth.login({
	    scope: 'talk_message,friends',
	    success: function(response) {
	        console.log(response);
	    },
	    fail: function(error) {
	        console.log(error);
	    }
  });*/
  
  // shareKakaotalk("BADA 인증 완료");

function Queue(){

    this.dataStore = [];
    this.enqueue = enqueue;
    this.dequeue = dequeue;
    this.search=search;
}

function enqueue(element)
{
    this.dataStore.push(element);
}

function dequeue()
{
    return this.dataStore.shift();
}

var cnt=0;
function search(){
    
    for(var i=0; i<this.dataStore.length;i++)
    {
      if((this.dataStore[i]-time)>1800000) //30분 이상이면 반복문탈출
      {
        break;
      }
      else{
        cnt++;
        console.log("Water events occur" + cnt + "times");
      }
    }
    if(cnt>=3)
    { console.log("Water event exceed 3 times. Send Message");
      return true;
    }
    else{
      return false;
    }
  }

w_remove = setInterval(function() {
    water.dequeue();
}, 600000);

var water= new Queue();


////////////////function tryConnectWebsocket() {

  // Connecting to ROS
  // -----------------
  var ros = new ROSLIB.Ros();

  // If there is an error on the backend, an 'error' emit will be emitted.
  ros.on('error', function (error) {
    // document.getElementById('connecting').style.display = 'none';
    // document.getElementById('connected').style.display = 'none';
    // document.getElementById('closed').style.display = 'none';
    // document.getElementById('error').style.display = 'inline';
    console.log(error);
  });

  // Find out exactly when we made a connection.
  ros.on('connection', function () {
    console.log('Connection made!');
    // document.getElementById('connecting').style.display = 'none';
    // document.getElementById('error').style.display = 'none';
    // document.getElementById('closed').style.display = 'none';
    // document.getElementById('connected').style.display = 'inline';
  });

  ros.on('close', function () {
    console.log('Connection closed.');
    // document.getElementById('connecting').style.display = 'none';
    // document.getElementById('connected').style.display = 'none';
    // document.getElementById('closed').style.display = 'inline';
  });

  // Create a connection to the rosbridge WebSocket server.
  ros.connect('ws://localhost:9090');

  // Like when publishing a topic, we first create a Topic object with details of the topic's name
  // and message type. Note that we can call publish or subscribe on the same topic object.
  var listener = new ROSLIB.Topic({
    ros: ros,
    name: '/listener',
    messageType: 'std_msgs/String'
  });

  // Then we add a callback to be called every time a message is published on this topic.
  listener.subscribe(function (message) {
    // console.log('Received message on ' + listener.name + ': ' + message.data);

    // If desired, we can unsubscribe from the topic as well.
    listener.unsubscribe();
  });

  // Like when publishing a topic, we first create a Topic object with details of the topic's name
  // and message type. Note that we can call publish or subscribe on the same topic object.
  var turtle1 = new ROSLIB.Topic({
    ros: ros,
    name: '/turtle1/pose',
    messageType: 'turtlesim/Pose'
  });

  // Then we add a callback to be called every time a message is published on this topic.
  var count = 0;
  turtle1.subscribe(function (message) {
    if (count) {
      console.log(message);
    }

    // console.log('received a turtlesim message');
    // console.log(message);
    x = message.x * 50;
    y = message.y * 50;
    theta = message.theta;
    // If desired, we can unsubscribe from the topic as well.
    // if(count>5000){
    //   turtle1.unsubscribe();
    // }
  });


  var signal = new ROSLIB.Topic({
    ros : ros,
    name : '/signal',
    messageType : 'std_msgs/String'
  });

  signal.subscribe(function(m){
    sig_name=m.data;


    //console.log("NOW SIGNAL : "+sig_name);
    document.getElementById("signal").innerHTML = sig_name;
    time=today.getTime();


    if(sig_name=='Water')
    {
      //먼저 검색해 
      if(water.search())
      { 
        //총 3번 이상 발생했다면
        for(var i=1; i<=3; i++)
        {
          water.dequeue();
        }
      
        cnt=0;
        shareKakaotalk(sig_name);
      }
      else
      {
       //발생한적없다면
       water.enqueue(time);
      }
    }
    else if(sig_name!="Silence")
    {
        shareKakaotalk(sig_name);
    }
  });
/////////////////////////////////////////////////






var ros = new ROSLIB.Ros({
  url: 'ws://localhost:9090'
});

var audio_topic = new ROSLIB.Topic({
  ros: ros,
  name: '/audio',
  messageType: 'std_msgs/String'
});

audio_topic.subscribe(function (m) {
  str = m.data;
  console.log(str);
  str = str.replace("\"", "").replace("\"", "");
  for(var i=0;i<100;i++) str = str.replace("\"","");
  str = str.substring(1);
  str = str.substring(1);
  str = str.slice(0,-1);
  str = str.slice(0,-1);
  //JSON.parse doesn't work. Failed to fix the syntax error.
  str = str.replace("[","");
  str = str.replace("]","");
  str = str.replace("[","");
  str = str.replace("]","");
  str = str.replace("[","");
  str = str.replace("]","");
  str = str.substring(0,str.indexOf("["));
  str = str.slice(0,-1);
  str = str.slice(0,-1);
  str = str.split(",");
  //console.log(str);

  document.getElementById("first_topic_name").innerHTML = str;
  document.getElementById("first_topic_proba").innerHTML=str[1];
  document.getElementById("second_topic_name").innerHTML=str[2];
  document.getElementById("second_topic_proba").innerHTML=str[3];
  document.getElementById("third_topic_name").innerHTML=str[4];
  document.getElementById("third_topic_proba").innerHTML=str[5];
  
});


function init() {
  canvas = document.getElementById('canvas');
  console.log('initializing canvas and websocket');
  console.log(canvas);
  ctx = canvas.getContext('2d');

  // Start the first frame request
  window.requestAnimationFrame(loop);
  tryConnectWebsocket();
}

function loop(timeStamp) {



  // Keep requesting new frames
  if (timeStamp - lastFrameTime < FRAME_MIN_TIME) { //skip the frame if the call is too early
    window.requestAnimationFrame(loop);
    return; // return as there is nothing to do
  }
  lastFrameTime = timeStamp; // remember the time of the rendered frame

  ctx.clearRect(0, 0, canvas.width, canvas.height);

  var secondsPassed = (timeStamp - oldTimeStamp) / 1000;
  var elapsedSeconds = (timeStamp - start) / 1000;
  oldTimeStamp = timeStamp;

  //Calculate fps
  var fps = Math.round(10 / secondsPassed) / 10;

  //Draw number to the screen
  ctx.fillStyle = 'white';
  ctx.fillRect(0, 0, 200, 100);
  ctx.font = '25px Arial';
  ctx.fillStyle = 'black';
  ctx.fillText("FPS: " + fps, 10, 30);

  draw(timeStamp);

  if (elapsedSeconds > 5) {
    return;
  } else {
    window.requestAnimationFrame(loop);
  }
}

function draw(timestamp) {
  // if (!start) start = timestamp;
  // var progress = timestamp - start;

  //Draw number to the screen
  const radius = 1500;
  const startangle = theta - 1 / 16 * Math.PI;
  const endangle = theta + 1 / 16 * Math.PI;
  ctx.fillStyle = 'rgba(255,100,50,0.3)';

  ctx.beginPath();//ADD THIS LINE!<<<<<<<<<<<<<
  ctx.moveTo(x, y);
  ctx.arc(x, y, radius, startangle, endangle);
  ctx.lineTo(x, y);
  ctx.fill(); // or context.fill()

  ctx.beginPath();//ADD THIS 
  ctx.fillStyle = 'red';//#DC143C
  ctx.arc(x, y, 5, 0, 2 * Math.PI);
  ctx.fill(); // or context.fill()
}

//from map.js

//Kakao.init("b886eede39b9d47bc9d3cb6e91483799");   // 사용할 앱의 JavaScript 키를 설정

function tryConnectWebsocket() {

    // Connecting to ROS
    // -----------------
    var ros = new ROSLIB.Ros();
  
    // If there is an error on the backend, an 'error' emit will be emitted.
    ros.on('error', function (error) {
      console.log(error);
    });
  
    // Find out exactly when we made a connection.
    ros.on('connection', function () {
      console.log('Connection made!');
    });
  
    ros.on('close', function () {
      console.log('Connection closed.');
    });
  
    // Create a connection to the rosbridge WebSocket server.
    ros.connect('ws://localhost:9090');
  
    // Like when publishing a topic, we first create a Topic object with details of the topic's name
    // and message type. Note that we can call publish or subscribe on the same topic object.
    var listener = new ROSLIB.Topic({
      ros: ros,
      name: '/listener',
      messageType: 'std_msgs/String'
    });
  
    listener.subscribe(function (message) {
      listener.unsubscribe();
    });
  
  
  
  
  
    var odom=new ROSLIB.Topic({
      ros:ros,
      name : '/t265/odom/sample/',
      messageType : 'nav_msgs/Odometry'
    });
  
  
  
    odom.subscribe(function (message) {
      // if (count) {
      //   console.log(message);
      // }
      x=message.pose.pose.position.x
      y=message.pose.pose.position.y
      //console.log(message.pose.pose.position);
      console.log(x,y);
      x=(x*55)+220;
      y=((y*55)+380);
  
      // X=(x*150) +280
       // 280, 230 = X*590
      //x=x*400;
      //y=y*400;
  
      var quaternion = new THREE.Quaternion(message.pose.pose.orientation.x, message.pose.pose.orientation.y, message.pose.pose.orientation.z, message.pose.pose.orientation.w);
      var euler= new THREE.Euler();
      euler.setFromQuaternion(quaternion,'XYZ');
      theta=euler.z;
  //   console.log(theta);
    });
  };
  
  
  
  
  ///////////////////////////
  var ros = new ROSLIB.Ros({
    url: 'ws://localhost:9090'
  });
  
  function init() {
    canvas = document.getElementById('canvas');
    console.log(canvas);
    ctx = canvas.getContext('2d');
  
    // Start the first frame request
    window.requestAnimationFrame(loop);
    tryConnectWebsocket();
  };
  
  function loop(timeStamp) {
    // Keep requesting new frames
    if (timeStamp - lastFrameTime < FRAME_MIN_TIME) { //skip the frame if the call is too early
      window.requestAnimationFrame(loop);
      return; // return as there is nothing to do
    }
    lastFrameTime = timeStamp; // remember the time of the rendered frame
    
    ctx.clearRect(0, 0, canvas.width, canvas.height);
  
    var secondsPassed = (timeStamp - oldTimeStamp) / 1000;
    var elapsedSeconds = (timeStamp - start) / 1000;
    oldTimeStamp = timeStamp;
  
    //Calculate fps
    var fps = Math.round(10 / secondsPassed) / 10;
  
    //Draw number to the screen
    //ctx.fillStyle = 'white';3
    //ctx.fillRect(0, 0, 200, 100);
    //ctx.font = '25px Arial';
    //ctx.fillStyle = 'black';
    //tx.fillText("FPS: " + fps, 10, 30);
  
    draw(timeStamp);
  
    if (elapsedSeconds > 5) {
      return;
    } else {
      window.requestAnimationFrame(loop);
    }
  }
  
  function draw(timestamp) {
    // if (!start) start = timestamp;
    // var progress = timestamp - start;
  
    //Draw number to the screen
   
    
    const radius = 1500;
    const startangle = theta - 1 / 16 * Math.PI;
    const endangle = theta + 1 / 16 * Math.PI;
    ctx.fillStyle = 'rgba(248,206,105,0.7)';
  
  
  
    ctx.beginPath();//ADD THIS LINE!<<<<<<<<<<<<<
    ctx.moveTo(x, y);
    ctx.arc(x, y, radius, startangle, endangle);
    ctx.lineTo(x, y);
    ctx.fill(); // or context.fill()
  
    ctx.beginPath();//ADD THIS 
    ctx.fillStyle = '#d45d54';//#DC143C
    ctx.strokeStyle="#black";
    ctx.lineWidth="10";
    ctx.arc(x, y, 7, 0, 2 * Math.PI);
    ctx.stroke();
    ctx.fill(); // or context.fill()*/
  }
  
   //canvas initiation

   /*-----------------------------------------from------------realtime_topic.js--------------------------------------------------------- */
   // Connecting to ROS
// -----------------
var ros = new ROSLIB.Ros();

// If there is an error on the backend, an 'error' emit will be emitted.
ros.on('error', function (error) {
    console.log(error);
});

// Find out exactly when we made a connection.
ros.on('connection', function () {
    console.log('Connection made!');
});

ros.on('close', function () {
    console.log('Connection closed.');
});

// Create a connection to the rosbridge WebSocket server.
ros.connect('ws://localhost:9090');

// Like when publishing a topic, we first create a Topic object with details of the topic's name
// and message type. Note that we can call publish or subscribe on the same topic object.
var listener = new ROSLIB.Topic({
    ros: ros,
    name: '/listener',
    messageType: 'std_msgs/String'
});

// Then we add a callback to be called every time a message is published on this topic.
listener.subscribe(function (message) {
    // console.log('Received message on ' + listener.name + ': ' + message.data);

    // If desired, we can unsubscribe from the topic as well.
    listener.unsubscribe();
});




var ros = new ROSLIB.Ros({
    url: 'ws://localhost:9090'
});

var audio_topic = new ROSLIB.Topic({
    ros: ros,
    name:'/audio',
    // name: '/bada_audio/audio',
    messageType: 'std_msgs/String'
});

audio_topic.subscribe(function (m){

    str = m.data;

    for (var i = 0; i < 100; i++) str = str.replace("\"", "");
    str = str.substring(1);
    str = str.substring(1);
    str = str.slice(0, -1);
    str = str.slice(0, -1);
    for (var i = 0; i < 100; i++) str = str.replace(",", "");
    for (var i = 0; i < 100; i++) str = str.replace("[", "");
	str=str.split("]");

    document.getElementById("first_topic").innerHTML = str[0];
    document.getElementById("second_topic").innerHTML = str[1];
    document.getElementById("third_topic").innerHTML = str[2];
    document.getElementById("fourth_topic").innerHTML = str[3];
    document.getElementById("fifth_topic").innerHTML = str[4];
    document.getElementById("sixth_topic").innerHTML = str[5];
    document.getElementById("seventh_topic").innerHTML = str[6];
    document.getElementById("eighth_topic").innerHTML = str[7];
    document.getElementById("ninth_topic").innerHTML = str[8];
    document.getElementById("tenth_topic").innerHTML = str[9];
});


function writeAlarmTest(){
  console.log('writing data to db...');
      
  // Make a request for a user with a given ID
  axios.put('/alarm', {alarmType:'knock', alarmTime: Date.now()} )
  .then(function (response) {
    // handle success
    // console.log(response);
    console.log(response.status);   // should be 200 (success)


  })
  .catch(function (error) {
    // handle error
    console.error(error);
  })
  .then(function () {
    // always executed
  });

}

function readAlarmTest(){
  console.log('reading data from db...');
      
  // Make a request for a user with a given ID
  axios.get('/alarm', {data:'what'} )
  .then(function (response) {
    // handle success
    // console.log(response);
    console.log(response.status);   // should be 200 (success)


  })
  .catch(function (error) {
    // handle error
    console.error(error);
  })
  .then(function () {
    // always executed
  });

}

function getAlarmTest(){
  console.log('getting data from db...');
      
  // Make a request for a user with a given ID
  axios.get('/alarm/1' )  // '/alarm/:id
  .then(function (response) {
    // handle success
    // console.log(response);
    console.log(response.status);   // should be 200 (success)


  })
  .catch(function (error) {
    // handle error
    console.error(error);
  })
  .then(function () {
    // always executed
  });

}
