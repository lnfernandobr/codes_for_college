const dgram = require("dgram");
const message = Buffer.from("fala fernandndin gato");

for (let i = 0; i < 50; ++i) {
  const client = dgram.createSocket("udp4");

  client.send(message, 7000, "localhost", err => {
    client.close();
  });
}
