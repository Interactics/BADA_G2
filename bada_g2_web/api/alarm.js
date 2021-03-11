var express    = require('express');
var router = express.Router();
var models = require('../models');
var axios = require('axios').default;
var util = require('../util')

router.get('/sendAlarm', async function(req, res){
  console.log('sending alarm to me')

  const { alarmType } = req.query
  let access_token=''
  try {
    access_token = req.cookies.access_token
    console.log('access_token: ', access_token)
    if(!access_token) throw 'TOKEN NULL'
  } catch (error) {
    console.warn('no access token. please login first')
    console.error(error)
    res.redirect('/api/auth')
  }

  try {
    const response = await axios.post(
      "https://kapi.kakao.com/v2/api/talk/memo/default/send", 
      `template_object={
        "object_type": "text",
        "text": "${alarmType || '알람'} 영역입니다. 최대 200자 표시 가능합니다.",
        "link": {
            "web_url": "http://localhost:3000/static",
            "mobile_web_url": "http://localhost:3000/static"
        },
        "button_title": "바로 확인"
    }`,
      {
        headers: {
          Authorization: `Bearer ${access_token.trim()}`
        }
        
      }
    )

    console.log(response)

    res.sendStatus(200)
  } catch (error) {
    console.error('FAILED')
    console.error(error)
    res.sendStatus(error.code)
  }
})

router.put('/', async function(req, res){

  console.log('writing alarm : ');
  // console.log(req.body);

  // const alarm = await models.Alarm.create({
  //   where: {
  //     alarmType: req.body.alarmType,
  //     time: req.body.alarmTime
  //   }
  // })

  const alarm = await models.Alarm.create({
    type: req.body.alarmType,
    time: req.body.alarmTime
  });


  // console.log(alarm);
  res.sendStatus(200);
})

router.get('/', async function(req, res){
  console.log('getting alarms');
  
  //const alarms = await models.Alarm.findAll({ });
  const alarms = await models.Alarm.findAll({limit:10, order: [['createdAt', 'DESC']]});
  // console.log(alarms);
  res.json(alarms);
})

router.get('/one', async function(req, res){
  console.log('getting one alarm');

  const alarm = await models.Alarm.findOne({ 
    where:{

    },order: [['createdAt', 'DESC']],
    
  })

  // console.log(alarm);

  res.json(alarm);

})

router.get('/:id', async function(req, res){
  console.log('getting specific alarm');

  const alarm = await models.Alarm.findOne({ 
    where:{

    },
    
  })

  // console.log(alarm);

  res.json(alarm);

})

module.exports = router;
