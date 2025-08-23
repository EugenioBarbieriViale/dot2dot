
class Game {
    constructor(n, r, w, h) {
        this.n = n;
        this.r = r

        this.w = w;
        this.h = h;

        this.dots = [];
        this.init_dots();

        this.canvas = document.getElementById("game");
        this.ctx = this.canvas.getContext("2d");

        this.canvas.addEventListener("mousedown", this.getMousePosition, false);

        this.getMousePosition = this.getMousePosition.bind(this);
        this.draw = this.draw.bind(this);
    }

    getMousePosition(event) {
        let x = event.pageX;
        let y = event.pageY;
        console.log(x + ", " + y);
    }

    init_dots() {
        for (let i = 0; i < this.n; i++) {
            for (let j = 0; j < (this.n - i); j++) {

                let x = 40 + (2 * this.r + 20) * j;
                let y = 60 + (2 * this.r + 10) * i;
                
                this.dots.push([x, y]);
            }
        }
    }

    draw() {
        this.ctx.clearRect(0, 0, this.w, this.h);
        this.ctx.save();

        this.ctx.fillStyle = "black";

        for (let dot of this.dots) {
            this.ctx.beginPath();
            this.ctx.arc(dot[0], dot[1], this.r, 0, Math.PI * 2);
            this.ctx.fill();
        } 

        this.ctx.restore();

        requestAnimationFrame(this.draw);
    }

    run() {
        this.draw();
    }
}

let w = 375;
let h = 375;

const game = new Game(6, 20, w, h);
game.run();
