let time = 0;

let timer = setInterval(function () {
  time += 2;
  console.log(time + ' seconds have passed');
  if (time > 3) {
    clearInterval(timer);
  }
}, 2000);

console.log(__dirname);
console.log(__filename);