package java.awt;

import java.awt.Component;
import java.awt.Graphics;
import java.awt.Rectangle;
import java.awt.event.PaintEvent;

/**
 *
 * Copyright (c) 1998
 *   Transvirtual Technologies Inc.  All rights reserved.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file.
 * @author P.C.Mehlitz
 */
class PaintEvt
  extends PaintEvent
{
	long issued;
	long ms;
	static PaintEvt cache;
	final static int REPAINT = 802;

PaintEvt ( Component src, int evtId, long ms, int x, int y, int width, int height ) {
	super( src, evtId, x, y, width, height);
	
	// we don't support "lazyness" control (ms is an *upper* bound)
	this.ms = ms;
	this.issued = -1;
}

protected void dispatch () {
	Component src = (Component)source;
	NativeGraphics g = NativeGraphics.getClippedGraphics( null, src, 0,0,
	                                                      x, y, width, height, true);	
	if ( g != null ){
		src.update( g);
		g.dispose();
	}
	
	recycle();
}

static synchronized PaintEvt getEvent ( Component source, int id,
                      long ms, int x, int y, int width, int height ) {
	if ( cache == null ) {
		return new PaintEvt( source, id, ms, x, y, width, height);
	}
	else {
		PaintEvt e = cache;
		cache = (PaintEvt) e.next;
		e.next = null;
		
		e.source = source;
		e.id = id;
		e.x = x;
		e.y = y;
		e.width = width;
		e.height = height;

		// we don't support "lazyness" control (ms is an *upper* bound)
		e.ms = ms;
		e.issued = -1;

		return e;
	}
}

static synchronized PaintEvt getEvent ( int srcIdx, int id, int x, int y, int width, int height ) {
	Component source = sources[srcIdx];
	Rectangle r;

	x += source.deco.x;
	y += source.deco.y;

	if ( cache == null ) {
		return new PaintEvt( source, id, 0, x, y, width, height);
	}
	else {
		PaintEvt e = cache;
		cache = (PaintEvt) e.next;
		e.next = null;
	
		e.source = source;
		e.id = id;
		e.ms = 0;
		e.issued = -1;
		e.x = x;
		e.y = y;
		e.width = width;
		e.height = height;

		return e;
	}
}

public Rectangle getUpdateRect () {
	return new Rectangle( x, y, width, height);
}

public String paramString() {
	String ps;
	
	switch ( id ) {
	case PAINT:   ps = "PAINT"; break;
	case UPDATE:  ps = "UPDATE"; break;
	case REPAINT: ps = "REPAINT"; break;
	default:      ps = ("unknown paint " + id);
	}

	ps += " [" + x + ',' + y + ',' + width + ',' + height + ']';

	return ps;
}

protected void recycle () {
	synchronized ( PaintEvt.class ) {
		source = null;

		next = cache;
		cache = this;
	}
}

public void setUpdateRect ( Rectangle r ) {
	x = r.x;
	y = r.y;
	width = r.width;
	height = r.height;
}

boolean solicitRepaint ( Component c, int cx, int cy, int cw, int ch ) {
	// our policy is not to propagate repaints upwards in the parent chain, since
	// this can cause rather big repaint areas, resulting in flicker
	if ( c != source ) return false;
	
	int xw = x + width;
	int yh = x + height;
	int cxw = cx + cw;
	int cyh = cy + ch;
	
	if ( cx < x ) x = cx;
	if ( cy < y ) y = cy;
	
	width = ((cxw > xw) ? cxw : xw) - x;
	height = ((cyh > yh) ? cyh : yh) - y;

	return true;
}
}
