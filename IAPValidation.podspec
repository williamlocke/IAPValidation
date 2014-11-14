Pod::Spec.new do |s|
  s.name         = "IAPValidation"

  s.version      = "0.0.2"

  s.summary      = "iOS In-App Purchase Validation - improved and modular version of Apple's VerificationController."

  s.homepage     = "https://github.com/williamlocke/IAPValidation"

	s.license      = { :type => 'FreeBSD', :file => 'LICENSE.txt' }

  s.author       = { "williamlocke" => "williamlocke@me.com" }

  s.source       = { :git => "https://github.com/williamlocke/IAPValidation.git", :tag => s.version.to_s }

  s.platform     = :ios, '5.0'
  
  s.source_files =  '*.[h,m]'
  
  
  s.frameworks = 'Security'

  s.requires_arc = true

end
