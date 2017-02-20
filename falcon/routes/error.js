var express = require('express');

module.exports = function(req, res, next){
  // we may use properties of the error object
  // here and next(err) appropriately, or if
  // we possibly recovered from the error, simply next().
  res.status(404);
  res.render('error', { error: req.originalUrl, title:"not found" });
};
