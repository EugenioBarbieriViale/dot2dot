
class Game {
    constructor(n, r, w, h) {
        this.n = n;
        this.r = r

        this.w = w;
        this.h = h;

        this.dots = [];
        this.initDots();

        this.canvas = document.getElementById("game");
        this.ctx = this.canvas.getContext("2d");

        this.canvas.addEventListener("mousedown", this.getMousePosition.bind(this));

        this.getMousePosition = this.getMousePosition.bind(this);
        this.draw = this.draw.bind(this);
        this.clicked = this.clicked.bind(this);
        // this.erase = this.erase.bind(this);
    }

    getMousePosition(event) {
        const rect = this.canvas.getBoundingClientRect();
        this.mouse_x = event.pageX - rect.left;
        this.mouse_y = event.pageY - rect.top;
    }

    clicked(dot) {
        const dx = this.mouse_x - dot[0];
        const dy = this.mouse_y - dot[1];
        const dist = Math.sqrt(dx * dx + dy * dy);
        return dist < this.r;
    }

    initDots() {
        for (let i = 0; i < this.n; i++) {
            for (let j = 0; j < (this.n - i); j++) {

                let x = 40 + (2 * this.r + 20) * j;
                let y = 60 + (2 * this.r + 10) * i;
                
                this.dots.push([x, y, false]);
            }
        }
    }

    draw() {
        this.ctx.clearRect(0, 0, this.w, this.h);
        this.ctx.save();

        for (let i=0; i<this.dots.length; i++) {
            if (this.clicked(this.dots[i])) {
                this.dots[i][2] = true;
            }

            let dot = this.dots[i];

            if (!dot[2]) {
                this.ctx.fillStyle = "black";
            }
            else {
                this.ctx.fillStyle = "white";
            }

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

window.onload = function() {
    const game = new Game(6, 20, w, h);
    game.run();
};
