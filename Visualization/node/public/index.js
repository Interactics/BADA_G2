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
var PrintHistory="Connection";

const today = new Date();
today.setTime(0);

var hidx=0;
const FRAMES_PER_SECOND = 10;  // Valid values are 60,30,20,15,10...
const FRAME_MIN_TIME = (1000 / 60) * (60 / FRAMES_PER_SECOND) - (1000 / 60) * 0.5;
var lastFrameTime = 0;  // the last frame time


Kakao.init("b886eede39b9d47bc9d3cb6e91483799");   // 사용할 앱의 JavaScript 키를 설정

function shareKakaotalk(sig_name)
{
    Kakao.API.request({
      url: '/v2/api/talk/memo/default/send',
      data: {
        template_object: {
          object_type: 'text',
          text: sig_name,
          link: {
              web_url: 'http://192.168.137.1',
              mobile_web_url: 'http://192.168.137.1',
            },
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
  shareKakaotalk("BADA 인증 완료");




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
    for (var i = this.dataStore.length-1;i >=0; --i )    {
        retStr += this.dataStore[i] + "\n";
    }
    return retStr;
}



var water= new Queue();

var h_element;
var h = new Queue();



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
  ros.connect('ws://192.168.0.6:9090');

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
  // 
  const dic1={'Speech':'말하는 소리', 'Alarm':'알람', 'Door':'노크', 'Television':'티비소리', 'Silence':'고요', 'Water':'물소리', 'Music':'음악'};
  
  signal.subscribe(function(m){
    sig_name=dic1[m.data];
    
    console.log("NOW SIGNAL : "+sig_name);
    document.getElementById("signal").innerHTML = sig_name;


    time=today.getTime();
    viewtime=printNow();

    if(h.dataStore.length>=5)
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
    document.getElementById("History").innerHTML=PrintHistory;

  });

}

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
  axios.get('/alarm/:1' )
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

function init() {
  canvas = document.getElementById('canvas');
  console.log(canvas);
  ctx = canvas.getContext('2d');

  // Start the first frame request
  window.requestAnimationFrame(loop);
  tryConnectWebsocket();
}

///////////////////////////
var ros = new ROSLIB.Ros({
  url: 'ws://localhost:9090'
});

var audio_topic = new ROSLIB.Topic({
  ros: ros,
  name: '/signal',
  messageType: 'std_msgs/String'
});

//////////////////////////////////////////////////////


//audio_topic.subscribe(function (m) {
  //str = m.data;
  //str = str.replace("\"", "").replace("\"", "");
  /*
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
  str = str.split(","); */
 // console.log(str);

 // document.getElementById("signal").innerHTML = str;
  /*
  document.getElementById("first_topic_proba").innerHTML=str[1];
  document.getElementById("second_topic_name").innerHTML=str[2];
  document.getElementById("second_topic_proba").innerHTML=str[3];
  */
//});

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



/*
function publishEncoded(topic, obj){
  var msg = new ROSLIB.Message({
    data: JSON.stringify(obj)
  });
  topic.publish(msg);
}

function decodeMessage(msg){
  return JSON.parse(msg.data);
}

audio_topic.subscribe(audio_topic) {
  var decoded = JSON.parse(msg.data);
  console.log(decoded);
});
*/













  // /**
  //  * Setup all visualization elements when the page is loaded.
  //  */
  // function init() {
  //   console.log('load');
  //   // Connect to ROS.
  //   var ros = new ROSLIB.Ros({
  //     url : 'ws://localhost:9090'
  //   });

  //   // Create the main viewer.
  //   var viewer = new ROS2D.Viewer({
  //     divID : 'map',
  //     width : 600,
  //     height : 500
  //   });

  //   // Setup the map client.
  //   var gridClient = new ROS2D.OccupancyGridClient({
  //     ros : ros,
  //     rootObject : viewer.scene
  //   });
  //   // Scale the canvas to fit to the map
  //   gridClient.on('change', function(){
  //     viewer.scaleToDimensions(gridClient.currentGrid.width, gridClient.currentGrid.height);
  //   });
  // }
