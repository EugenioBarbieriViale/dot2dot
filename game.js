class Game {
    constructor(canvas, n, r, w, h) {
        this.n = n;
        this.r = r

        this.w = w;
        this.h = h;

        this.dots = [];
        this.initDots();

        this.temp = 0;
        this.turn = 0;
        this.end = 0;

        this.pair = [];

        this.canvas = canvas;
        this.ctx = this.canvas.getContext("2d");
        this.ctx.font = "30px Helvetica";

        canvas.addEventListener("mousedown", this.getMousePosition.bind(this));

        this.clicked = this.clicked.bind(this);
        this.erase = this.erase.bind(this);
        this.run = this.run.bind(this);
        this.showText = this.showText.bind(this);
    }

    initDots() {
        let c = 0;
        for (let i = 0; i < this.n; i++) {
            for (let j = 0; j < (this.n - i); j++) {

                let x = 40 + (2 * this.r + 20) * j;
                let y = 85 + (2 * this.r + 10) * i;
                
                this.dots.push([x, y, false, c, false]);
                c++;
            }
        }
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

    erase() {
        if (this.pair.length == 0) {
            return;
        }

        if (this.pair[0][1] != this.pair[1][1]) {
            this.end = 21;
            return;
        }
        
        let i1 = this.pair[0][3];
        let i2 = this.pair[1][3];

        if (i1 > i2) {
            i2 = [i1, i1 = i2][0];
        }

        for (let i=i1; i <= i2; i++) {
            if (this.dots[i][2]) {
                this.end = 21;
                break;
            }

            this.dots[i][2] = true;
            this.end++;
        }
    }

    showText() {
        let player_msg = "";
        if (this.turn % 2 == 0) {
            player_msg = "player 1";
        }
        else {
            player_msg = "player 2";
        }

        if (this.end == 21) {
            this.ctx.font = "40px Helvetica";
            this.ctx.fillStyle = "#000000";
            this.ctx.fillText(player_msg + " won", 70, 200);
        }
        else {
            this.ctx.save();
            this.ctx.fillText(player_msg, 130, 38);
            this.ctx.fillText("dot2dot", 260, 350);
            this.ctx.restore();
        }
    }

    run() {
        this.ctx.clearRect(0, 0, this.w, this.h);
        this.ctx.save();

        for (let i=0; i<this.dots.length; i++) {
            if (this.clicked(this.dots[i])) {
                this.dots[i][4] = true;

                this.pair.push(this.dots[i]);
                this.temp++;

                if (this.temp % 2 == 0) {
                    this.erase();
                    this.pair = [];

                    this.turn++;
                }

                this.mouse_x = 0;
                this.mouse_y = 0;
            }

            let dot = this.dots[i];

            if (this.end != 21) {
                if (!dot[2] && !dot[4]) {
                    this.ctx.fillStyle = "black";
                }
                else if (dot[2]) {
                    this.ctx.fillStyle = "white";
                }
                else {
                    this.ctx.fillStyle = "grey";
                }

                this.ctx.beginPath();
                this.ctx.arc(dot[0], dot[1], this.r, 0, Math.PI * 2);
                this.ctx.fill();
            }
        } 
        this.ctx.restore();

        this.showText();

        requestAnimationFrame(this.run);
    }
}
