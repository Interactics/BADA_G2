const express = require('express');
const models = require('../models');
// require('manakin').global;

const router = express.Router();
let initiated = false
async function init(){
    console.log('creating robot');

    const robot = await models.Robot.create({
        id:1,
        x:900,
        y:600,
        dir:50,
    })

    console.info(robot);

    return robot
}

router.put('/', async function(req, res){
    console.log('writing robot');
    // console.log(req.body);

    const robot= await models.Robot.findOne({
        where: {
            id: 1
        }
    })

    if(!robot) return;

    await robot.update({
        id:1,
        x: req.body.x*2+230,
        y: req.body.y*2+180,
        dir: req.body.dir,
    })

    console.log(robot);
    res.sendStatus(200);
})


router.get('/', async function(req, res){
    console.log('get robot satte');
    // await init()
    // const robot = await models.Robot.findOne({
    //     where:{
    //         id:1
    //     }
    // })
    const robot = await models.Robot.findByPk(1)

    // console.log(robot);
    if(!robot){
        
        console.error('ROBOT NULL')
        console.error(robot)
        res.json(await init())
        return;
    }
    res.json(robot);

})

module.exports = router;
