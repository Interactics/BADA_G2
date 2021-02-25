const client = require('axios').default
const REDIRECT_URI = `http://localhost:3000/api/auth/success`
const auth = require('express').Router()
const axios = require('axios').default


auth.get('/', async function(req, res){
  console.log('trying kakao login');
  // res.render('index');
  // const response = await client.get(`https://kauth.kakao.com/oauth/authorize?client_id=a193697762d7294f8e0080484325aa14&redirect_uri=/api/auth/success&response_type=code`)
  // console.info(response)

  res.redirect(`https://kauth.kakao.com/oauth/authorize?client_id=a193697762d7294f8e0080484325aa14&redirect_uri=http://localhost:3000/api/auth/callback&response_type=code`)
});

auth.get('/callback', function(req, res){
  console.log('kakao login success')
  // res.render('index');
  console.log(req.query)
  const { access_token } = req.query
  res.sendStatus(200)
});

auth.get('sendme', function(req, res){
  console.log('sending msg to me')

  axios.post(
    "https://kapi.kakao.com/v2/api/talk/memo/default/send", 
    {
          "object_type": "text",
          "text": "텍스트 영역입니다. 최대 200자 표시 가능합니다.",
          "link": {
              "web_url": "https://developers.kakao.com",
              "mobile_web_url": "https://developers.kakao.com"
          },
          "button_title": "바로 확인"
      },
      {
        headers: [
          "Authorization: Bearer {ACCESS_TOKEN}"
        ]
      }
    // -H "Authorization: Bearer {ACCESS_TOKEN}" \
    // -d 'template_object={
    //     "object_type": "text",
    //     "text": "텍스트 영역입니다. 최대 200자 표시 가능합니다.",
    //     "link": {
    //         "web_url": "https://developers.kakao.com",
    //         "mobile_web_url": "https://developers.kakao.com"
    //     },
    //     "button_title": "바로 확인"
    // }'
  )
})

// GET /oauth/authorize?client_id={REST_API_KEY}&redirect_uri={REDIRECT_URI}&response_type=code HTTP/1.1
// Host: kauth.kakao.com

module.exports = auth