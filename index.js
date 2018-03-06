var include = [__dirname];

module.exports = {
  include: include.map(function(item) {
    return '"' + item + '"';
  }).join(' ')
};
