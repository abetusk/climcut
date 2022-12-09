// LICENSE: cc0
//

function allready() {
  console.log(">>>go");
}

var aaa = allready;

var ok = require("./a.out.js");
var ko = new require("./a.out.js");

function checkf() {
  if (typeof ok.getBytes === "undefined") {
    console.log("nope"); 
    setTimeout(checkf, 1);
    return;
  }

  console.log("ok!");
  _main();

}

// https://stackoverflow.com/a/46154154 by https://stackoverflow.com/users/1447675/nina-scholz
// CC-BY-SA
//
function getFloat(a) {
  let view = new DataView(new ArrayBuffer(8));
  a.forEach(function (b, i) { view.setUint8(i, b); });
  return view.getFloat64(0);
}

function _main() {

  let iv_ptr = ok._malloc(4*4);
  let dv_ptr = ok._malloc(8*4);
  let dv_ptr1 = ok._malloc(4, 8);

  ok.ccall("funco", 'number', ['number', 'number', 'number', 'number'], [iv_ptr, 4, dv_ptr, 4]);
  ok.ccall("funco", 'number', ['number', 'number', 'number', 'number'], [iv_ptr, 4, dv_ptr1, 4]);

  let z0 = ok.HEAPF64.subarray(dv_ptr>>2, (dv_ptr>>2) + 4);
  console.log("z0:", z0);

  let z1 = ok.HEAPF64.subarray(dv_ptr1>>4, (dv_ptr1>>4) + 4);
  console.log("z1:", z1);

  let z2 = ok.HEAPF64.subarray(dv_ptr>>1, (dv_ptr>>1) + 4);
  console.log("z2:", z2);

  let z3 = ok.HEAPF64.subarray(dv_ptr1>>3, (dv_ptr1>>3) + 4);
  console.log("z3:", z3);

  ok._free(iv_ptr);
  ok._free(dv_ptr);
  ok._free(dv_ptr1);

  ok.ccall('funcu', 'number', []);

  return;

  var d = ok.getDoubles();
  for (let i=0; i<d.length; i++) {
    console.log(i, d[i]);
  }

  return;
  /* console.log("rawdat:", rawdat.length);

  let d = [];
  for (let i=0; i<rawdat.length; i+=8) {
    let s = rawdat.slice(i,i+8);
    s.reverse();
    console.log(i, s);
    d.push(getFloat(s));
  }

  console.log(d);
  */

}

setTimeout( checkf, 1);


//console.log("????", ok.getBytes);
//console.log(ok.calledRun);
//console.log( JSON.stringify(ok, null, 2) );

//setTimeout( function() { console.log("...",  ok.getBytes ); }, 2000 );

/*
console.log(ok.Module);
console.log(ko.Module);
console.log(ok);
console.log(ko);
console.log(ko.getBytes);
console.log(ok.getBytes);
console.log(ok.getBytes());
*/
