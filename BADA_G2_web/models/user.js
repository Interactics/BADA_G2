const { Sequelize } = require("sequelize");

module.exports = function (sequelize, DataTypes) {
  const user = sequelize.define('User', {
    google_id: { field: 'google_id', type: DataTypes.STRING(50), unique: true, allowNull: true },
    state: { field: 'state', type: DataTypes.INTEGER, allowNull: false, defaultValue:0 }
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
    tableName: 'user'
  });

  return user;
};

/*
 Sequelize 참고
 DataTypes => http://docs.sequelizejs.com/en/v3/api/datatypes/
 Associations => http://docs.sequelizejs.com/en/v3/api/associations/
 Model Function => http://docs.sequelizejs.com/en/v3/api/model/
 */