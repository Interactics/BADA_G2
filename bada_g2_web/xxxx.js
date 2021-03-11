const express = require('express');
const models = require('./models');
const ROSLIB = require('roslib');
const axios = require('axios').default;

const BASE_URL = `http://localhost:3000/api`


function tryConnectWebsocket() {

  // Connecting to ROS
  // -----------------
  var ros = new ROSLIB.Ros();

  // If there is an error on the backend, an 'error' emit will be emitted.
  ros.on('error', function (error) {
    connectState = -1;
    // document.getElementById('connecting').style.display = 'none';
    // document.getElementById('connected').style.display = 'none';
    // document.getElementById('closed').style.display = 'none';
    // document.getElementById('error').style.display = 'inline';
    console.error(error);
  });

  // Find out exactly when we made a connection.
  ros.on('connection', function () {
    console.info('Connection made!');
    connectState=1
    // document.getElementById('connecting').style.display = 'none';
    // document.getElementById('error').style.display = 'none';
    // document.getElementById('closed').style.display = 'none';
    // document.getElementById('connected').style.display = 'inline';
  });

  ros.on('close', function () {
    console.warn('Connection closed.');
    connectState = -1
    // document.getElementById('connecting').style.display = 'none';
    // document.getElementById('connected').style.display = 'none';
    // document.getElementById('closed').style.display = 'inline';
  });

  // Create a connection to the rosbridge WebSocket server.
  // ros.connect('ws://socket.bada.epikem.com:80');
  ros.connect('ws://localhost:9090');

  // Like when publishing a topic, we first create a Topic object with details of the topic's name
  // and message type. Note  that we can call publish or subscribe on the same topic object.

  odom = new ROSLIB.Topic({
    ros: ros,
    name: '/bada/filtered_odom',
    messageType: 'nav_msgs/Odometry'
  });

  const audioListener = new ROSLIB.Topic({
    ros: ros,
    name: '/bada/audio/signal',
    //name : '/bada_audio/signal',
    messageType: 'std_msgs/String'
  });

  audioListener.subscribe(function (m) {
    console.info('audio signal topic called!')
    console.info(m)
    console.info(m.data)
    
    try {
      axios.get(`${BASE_URL}/alarm/sendAlarm?alarmType=${m.data}`)
    } catch (error) {
      console.error(error)
    }

    // sig_name = dic1[m.data];
    // console.log("NOW SIGNAL : " + sig_name);



    // time = today.getTime();
    // var a= new Date();
    // viewtime = printNow(a);

    // if (h.dataStore.length >= 17) {
    //   h.dequeue();
    // }

    // if (sig_name == dic1['Water']) {
    //   //먼저 검색해 
    //   if (water.search()) {
    //     //총 3번 이상 발생했다면
    //     for (var i = 1; i <= 3; i++) {
    //       water.dequeue();
    //     }
    //     cnt = 0;
    //     shareKakaotalk(m.data);
    //     h.enqueue([sig_name, viewtime]);
    //     hidx = hidx + 1;
    //     PrintHistory = h.toString();
    //   }
    //   else {
    //     //발생한적없다면
    //     water.enqueue(time);
    //   }
    // }
    // else if (sig_name != dic1["Silence"]) {
    //   console.log('signame!!::', sig_name);
    //   shareKakaotalk(m.data);
    //   h.enqueue([sig_name, viewtime]);
    //   hidx = hidx + 1;
    //   PrintHistory = h.toString();
    // }
    // document.getElementById("history").innerHTML = PrintHistory;
    // document.getElementById("alarm").innerHTML = sig_name;

    // if(m.data=='Speech') document.getElementById("sign_language_gif").src='./말소리';
    // else if(m.data=='Alarm') document.getElementById("sign_language_gif").src='./화재경보기.gif';
    // else if(m.data=='Door') document.getElementById("sign_language_gif").src='./노크.gif';
    // else if(m.data=='Boiling') document.getElementById("sign_language_gif").src='./물+끓다.gif';
    // else if(m.data=='Cry') document.getElementById("sign_language_gif").src='./아기+울음소리.gif';
    // else if(m.data=='Bell') document.getElementById("sign_language_gif").src='./초인종.gif';
    // else if(m.data=='Water') document.getElementById("sign_language_gif").src='./물.gif';

  });

}


tryConnectWebsocket()