var path = require('path');
var bodyParser = require('body-parser');
var express = require('express');
const env = process.env.NODE_ENV || 'development';
const config = require(__dirname + '/config/config.json')[env];
const cookieParser = require('cookie-parser')


var app = express();
app.use(bodyParser.json());
app.use(cookieParser())

var staticPath = path.join(__dirname, '/public');

var api = require('./api');

app.engine('html', require('ejs').renderFile);
app.set('view engine', 'html');

app.use('/static', express.static(staticPath));

app.use('/api', api);


// connect To DB
const models = require('./models');
models.sequelize.sync({force: config.forceSync})
  .then(() => {
    console.log('✓ DB connection success.');
    console.log('  Press CTRL-C to stop\n');
  })
  .catch(err => {
    console.error(err);
    console.log('✗ DB connection error. Please make sure DB is running.');
    process.exit();
  });

require('./ros')

app.listen(3000, function() {
  console.log('listening');
});
