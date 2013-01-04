#!/usr/bin/env ruby

require 'hornetseye_v4l2'
require 'hornetseye_rmagick'

class Camera
  def initialize
    @cam = Hornetseye::V4L2Input.new '/dev/video0' do |modes|
      areas = modes.map { |mode| [mode[1] * mode[2], mode] }
      areas.sort.last[1]
    end
    30.times { @cam.read }

    @capture = -1
    poll
  end

  def filename
    "capture_%06d.jpeg" % @capture
  end

  def capture
    @capture += 1

    print "Capture #{@capture}... "

    frame = @cam.read
    img = frame.to_type Hornetseye::UBYTERGB
    img.save_ubytergb filename
    puts 'Done.'
  end

  def poll
    # Poll the camera, to keep it adjusting to the lighting.
    Thread.new do
      loop do
        sleep 1
        @cam.read
      end
    end
  end
end

camera = Camera.new

Thread.new { sleep 5; exit }

loop do
  camera.capture
  sleep 0.1
end
