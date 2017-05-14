# mruby-bsdevdev   [![Build Status](https://travis-ci.org/yamori813/mruby-bsdevdev.svg?branch=master)](https://travis-ci.org/yamori813/mruby-bsdevdev)
BsdEvdev class
## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'yamori813/mruby-bsdevdev'
end
```
## example
```ruby
t = BsdEvdev.new(0)
p t.getsw
#=> 7
```

## License
under the BSD License:
- see LICENSE file
