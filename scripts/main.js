// console.print("hello world form js");

// const a = 10;
// const b = 20;

// console.print("sum =", a + b);

server.get("/hellos", function (req, res) {
    res.send("hello world there ");
});

server.get("/like", function (req, res) {
    console.print(req.method);
    res.send("thanks ^_^");
})
server.post("/postreq", function (req, res) {
    console.print(req.body);
    res.status(401).send(`what here ? ${req.body}`);
})