const models = require('../models')

module.exports.saveToken = async function (access_token, refresh_token){
  console.log('writing token');

  const tokens= await models.Auth.findOne({
      where: {
          access_token
      }
  })

  if(tokens) {
    console.info('token already ex')
    return tokens;
  }
  
  const result = await models.Auth.create({
    access_token,
    refresh_token
  })

  console.log(result);
  return result
}


module.exports.getToken = async function (){
  console.log('getting token');

  const tokens= await models.Auth.findAll()

  if(tokens.length===0) {
    console.error('NO TOKEN')
    return {}
  }
  
  return tokens[0]
}
