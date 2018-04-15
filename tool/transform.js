const readline = require('readline');

const rl = readline.createInterface({
  input: process.stdin,
  ouput: process.stdout
});

let state = 0;

rl.on('line', (input) => {
  if (state === 0) {
  } else if (state === 1) {
    input.split(' --> ').map((data) => {
      console.log(data);
    });
  } else if (state === 2) {
    console.log(input);
  } else if (state === 3) {
  }
  state = (state + 1) % 4;
});
