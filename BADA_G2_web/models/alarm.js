const { Sequelize } = require("sequelize");

module.exports = function (sequelize, DataTypes) {
  const alarm = sequelize.define('Alarm', {
    type: { field: 'type', type: DataTypes.STRING(50), unique: true, allowNull: true },
    // time: { field: 'time', type: DataTypes.DATE, allowNull: false, defaultValue:Sequelize.NOW }
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
    tableName: 'alarm'
  });

  return alarm;
};

/*
 Sequelize 참고
 DataTypes => http://docs.sequelizejs.com/en/v3/api/datatypes/
 Associations => http://docs.sequelizejs.com/en/v3/api/associations/
 Model Function => http://docs.sequelizejs.com/en/v3/api/model/
 */