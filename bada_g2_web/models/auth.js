const { Sequelize } = require("sequelize");

module.exports = function (sequelize, DataTypes) {
  const auth = sequelize.define('Auth', {
    access_token: { field: 'access_token', type: DataTypes.STRING(200), unique: false, allowNull: true },
    refresh_token: { field: 'refresh_token', type: DataTypes.STRING(200), unique: false, allowNull: true },
  }, {
    // don't use camelcase for automatically added attributes but underscore style
    // so updatedAt will be updated_at
    underscored: true,

    // indexes: [
    //   {unique:true,
    //   fields:['mac']}
    // ],
    // disable the modification of tablenames; By default, sequelize will automatically
    // transform all passed model names (first parameter of define) into plural.
    // if you don't want that, set the following
    freezeTableName: true,

    // define the table's name
    tableName: 'auth'
  });

  return auth;
};


/*
 Sequelize 참고
 DataTypes => http://docs.sequelizejs.com/en/v3/api/datatypes/
 Associations => http://docs.sequelizejs.com/en/v3/api/associations/
 Model Function => http://docs.sequelizejs.com/en/v3/api/model/
 */