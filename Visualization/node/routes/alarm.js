var express    = require('express');
var router = express.Router();
var models = require('../models');

router.put('/', async function(req, res){
  console.log('writing alarm : ');
  console.log(req.body);

  const alarm = await models.Alarm.create({
    where: {
      type: req.body.alarmType,
      // time: req.body.alarmTime
    }
  })

  console.log(alarm);
  
  res.sendStatus(200);
});

router.get('/', async function(req, res){
  console.log('getting alarms');
  
  const alarms = await models.Alarm.findAll({ });

  console.log(alarms);

  res.json(alarms);
})

router.get('/:id', async function(req, res){
  console.log('getting specific alarm');

  const alarm = await models.Alarm.findOne({ 
    where:{

    }
  })

  console.log(alarm);

  res.json(alarm);

})

module.exports = router;
