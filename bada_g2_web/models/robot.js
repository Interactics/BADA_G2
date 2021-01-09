const { Sequelize } = require("sequelize");

module.exports = function (sequelize, DataTypes) {
  const robot = sequelize.define('Robot', {
    id: { field: 'id', type: DataTypes.INTEGER(11), unique: true, primaryKey: true, allowNull: false },
    x: { field: 'x', type: DataTypes.DOUBLE, unique: false, allowNull: true },
    y: { field: 'y', type: DataTypes.DOUBLE, allowNull: false, defaultValue:Sequelize.NOW },
    dir: { field: 'dir', type: DataTypes.DOUBLE, unique: false, allowNull: true }
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
    tableName: 'robot'
  });

  return robot;
};

/*
 Sequelize 참고
 DataTypes => http://docs.sequelizejs.com/en/v3/api/datatypes/
 Associations => http://docs.sequelizejs.com/en/v3/api/associations/
 Model Function => http://docs.sequelizejs.com/en/v3/api/model/
 */