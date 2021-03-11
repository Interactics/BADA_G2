const client = require('axios').default
const REDIRECT_URI = `http://localhost:3000/api/auth/success`
const auth = require('express').Router()
const axios = require('axios').default
const querystring = require('querystring')
const env = process.env.NODE_ENV || 'development';
const config = require(__dirname + '/../config/config.json')[env];
const models = require('../models');
const util = require('../util');

const BASE_URL=`https://kauth.kakao.com/oauth`

auth.get('/', async function(req, res){
  console.log('trying kakao login');
  // res.render('index');
  // const response = await client.get(`https://kauth.kakao.com/oauth/authorize?client_id=a193697762d7294f8e0080484325aa14&redirect_uri=/api/auth/success&response_type=code`)
  // console.info(response)

  res.redirect(`${BASE_URL}/authorize?client_id=${config.kakao_rest_api_key}&redirect_uri=http://localhost:3000/api/auth/callback&response_type=code`)
});

auth.get('/callback', async function(req, res){
  console.log('kakao login wip')
  // res.render('index');
  // console.log(req.query)
  const { code } = req.query

  const data = querystring.stringify({
    grant_type: 'authorization_code',
    client_id: config.kakao_rest_api_key,
    redirect_uri: 'http://localhost:3000/api/auth/callback',
    code: code,
  })

  try {
    const response = await axios.post(`${BASE_URL}/token`, 
       data,{
      headers: { 'Content-type': 'application/x-www-form-urlencoded;charset=utf-8' }
    },
  
    )
  
    // console.log(response)

    const { access_token, refresh_token, token_type, scope } = response.data

    res.cookie('access_token',access_token, { httpOnly: true, secure: true, maxAge: 3600000 })
    res.cookie('refresh_token',refresh_token, { httpOnly: true, secure: true, maxAge: 3600000 })

    util.saveToken(access_token, refresh_token)

    res.sendStatus(200)
  } catch (error) {
    console.error(error)
    throw error
  }
});





auth.get('/sendme', async function(req, res){
  console.log('sending msg to me')

  const { param } = req.query
  let access_token=''
  try {
    access_token = req.cookies.access_token
    console.log('access_token: ', access_token)
  } catch (error) {
    console.warn('no access token. please login first')
    res.redirect('/api/auth')
  }

  try {
    const response = await axios.post(
      "https://kapi.kakao.com/v2/api/talk/memo/default/send", 
      `template_object={
        "object_type": "text",
        "text": "${param || '알람'} 영역입니다. 최대 200자 표시 가능합니다.",
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

auth.get('/done', (req,res)=>{
  console.info('done!')
  res.sendStatus(200)
})

module.exports = auth