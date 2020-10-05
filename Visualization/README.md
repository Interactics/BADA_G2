# simple ros2d visualization app

## description

this app provides a static html file that connects to a rosbridge server via a websocket.
the html file is served by a simple express web server.

## installation

1. install `nodejs`
2. install node package manager: `npm` or `yarn`
3. install node_modules by running below command in `RobotCapstone/Visualization/ros2d` folder:

<details><summary markdown="span">npm</summary>

```
npm install
```

</details>


<details><summary markdown="span">yarn</summary>

```
yarn
```

</details>


4. run express server:

<details><summary markdown="span">npm</summary>

```
npm run start
```

</details>

<details><summary markdown="span">yarn</summary>

```
yarn start
```

</details>

## connecting to a rosbridge websocket server

1. run rosbridge websocket server by running these commands on *separate* terminals:
> 📂 `terminal1`
```
roscore
```
> 📂 `terminal2`
```
rosrun rosbridge_server rosbridge_websocket.py
```

2. run express server:
> 📂 `terminal3:RobotCapstone/Visualization/ros2d`
```
cd ./Visualization/ros2d
```

<details><summary markdown="span">npm</summary>

```
npm run start
```

</details>


<details><summary markdown="span">yarn</summary>

```
yarn start
```

then open url `http://localhost:3000` on browser

</details>

3. play a rosbag file:
> 📂 `terminal4`
```
rosbag play <bagfile path>
```

the browser developer console should show published ros topic information when connected to websocket server.

## stack

- nodejs
- express
- roslib