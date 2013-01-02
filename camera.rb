#!/usr/bin/env ruby

require 'hornetseye_v4l2'
require 'hornetseye_rmagick'
   
cam = Hornetseye::V4L2Input.new '/dev/video0' do |modes|
  areas = modes.map { |mode| [mode[1] * mode[2], mode] }
  areas.sort.last[1]
end
10.times { cam.read }
 
capture = -1

Signal.trap("USR1") do
  capture += 1

  frame = cam.read
  img = frame.to_type Hornetseye::UBYTERGB
  img.save_ubytergb "capture_#{capture}.jpeg"
end

sleep
