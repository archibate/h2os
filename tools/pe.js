'use strict';

class Render
{
	var canvas, ctx;

	constructor(canvas) {
		this.canvas = canvas;
		this.ctx = this.canvas.getContext('2d');
	}

	get width() {
		return this.canvas.width;
	}

	get height() {
		return this.canvas.height;
	}

	set width(w) {
		this.canvas.width = w;
	}

	set height(h) {
		this.canvas.height = h;
	}

	clear() {
		this.ctx.fillRect(0, 0, this.width, this.height);
	}
};

var render = new Render(document.getElementById('#canvas'));
render.clear();
