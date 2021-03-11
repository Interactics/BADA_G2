const saveToken = require('./token').saveToken
const getToken = require('./token').getToken
const sendAlarm = require('./sendAlarm').sendAlarm

module.exports = {
  saveToken,
  getToken,
  sendAlarm
}