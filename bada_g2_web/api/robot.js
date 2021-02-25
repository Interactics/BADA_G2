const express = require('express');
const models = require('../models');

const router = express.Router();

async function init(){
    console.log('creating robot');

    const robot = await models.Robot.create({
        id:1,
        x:0,
        y:0,
        dir:0,
    })

    console.log(robot);
}

init();

router.put('/', async function(req, res){
    console.log('writing robot');
    console.log(req.body);

    const robot= await models.Robot.findOne({
        where: {
            id: 1
        }
    })

    if(!robot) return;

    await robot.update({
        id:1,
        x: req.body.x,
        y: req.body.y,
        dir: req.body.dir,
    })

    console.log(robot);
    res.sendStatus(200);
})


router.get('/', async function(req, res){
    console.log('get robot satte');

    const robot = await models.Robot.findOne({
        where:{
            id:1
        }
    })

    console.log(robot);
    res.json(robot);

})

module.exports = router;
