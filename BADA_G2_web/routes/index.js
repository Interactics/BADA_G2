var express    = require('express');
var router = express.Router();

router.get('/', function(req, res){
  console.log('someone connect your page');
  // res.render('index');
  res.sendStatus(200);
});

module.exports = router;
