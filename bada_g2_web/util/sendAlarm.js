const getToken = require('./token').getToken
const axios = require('axios').default

module.exports.sendAlarm = async function (alarmType) {

  console.log('sending alarm to me')

  // const { alarmType } = req.query
  let token = await getToken()
  try {
    access_token = token.access_token
    console.log('access_token: ', access_token)
    if(!access_token) throw 'TOKEN NULL'
  } catch (error) {
    console.warn('no access token. please login first')
    console.error(error)
    throw error
  }

  try {

    var q=new Date()
    axios.put('http://localhost:3000/api/alarm', {
      alarmType,
      alarmTime: q
    })
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

    // console.log(response)

    // res.sendStatus(200)
    return response
  } catch (error) {
    console.error('FAILED')
    console.error(error)
    res.sendStatus(error.code)
  }
}