var express    = require('express');
var api = express.Router();
var Ros = require('roslib').Ros;
var auth = require('./auth');
var alarm = require('./alarm');
var robot = require('./robot');

api.get('/', function(req, res){
  console.log('someone connect your page');
  // res.render('index');
  res.sendStatus(200);
});

api.use('/auth', auth);
api.use('/alarm', alarm);
api.use('/robot', robot);

module.exports = api;
