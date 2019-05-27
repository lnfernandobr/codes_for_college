const http = require("http");
const cluster = require("cluster");
const numCPUs = require("os").cpus().length;
const udp = require("dgram");

const port = 7000;
const host = "localhost";

if (cluster.isMaster) {
  masterProcess();
} else {
  childProcess();
}



function masterProcess() {
  // console.log(`Master ${process.pid} is running`);

  for (let i = 0; i < numCPUs; i++) {
    cluster.fork();
  }
}

function childProcess() {
  const server = udp.createSocket("udp4");

  server.on('error', (err) => {
    console.log(`server error:\n${err.stack}`);
    server.close();
  });

  server.on('message', (msg, rinfo) => {
    console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}, pid = ${process.pid}`);
  });

  server.on('listening', () => {
    const address = server.address();
    console.log(`server listening ${address.address}:${address.port}`);
  });

  server.bind(port, host);

  console.log(`Worker ${process.pid} started and finished`);

}
