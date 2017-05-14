##
## BsdEvdev Test
##

assert("BsdEvdev#hello") do
  t = BsdEvdev.new "hello"
  assert_equal("hello", t.hello)
end

assert("BsdEvdev#bye") do
  t = BsdEvdev.new "hello"
  assert_equal("hello bye", t.bye)
end

assert("BsdEvdev.hi") do
  assert_equal("hi!!", BsdEvdev.hi)
end
