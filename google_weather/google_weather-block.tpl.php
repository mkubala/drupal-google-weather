<?php // $Id: google_weather.module,v 1.0 2010/07/10 19:06:00 m.kubala Exp $ ?>
<div class="weather-wrapper">
  <div class="weather current">
    <div class="location"><?php print $content['info']['location']; ?></div>
    <div class="weather-icon float-left">
      <?php print $content['current']['icon']; ?>
    </div>
    <div class="weather-info forecast-info float-left">
      <b><?php print t('Teraz'); ?></b><br/>
      <div class="temp"><?php print $content['current']['temp']; ?>°C</div>
      <div class="condition"><?php print $content['current']['condition']; ?></div>
      <div class="wind"><?php print $content['current']['wind_condition']; ?></div>
    </div>
    <div class="clear-block"></div>
  </div>

  <div class="separator">
    <?php print t("Prognoza pogody"); ?>
  </div>

  <?php foreach($content['forecast'] as $id => $forecast): ?>
    <div class="weather forecast forecast-<?php print $id; ?>">
      <div class="weather-icon float-left">
        <?php print $forecast['icon']; ?>
      </div>
      <div class="weather-info forecast-info float-left">
        <b><?php print $forecast['day']; ?></b><br/>
        <?php print $forecast['high_temp']; ?>°C | <?php print $forecast['low_temp']; ?>°C<br/>
        <?php print $forecast['condition']; ?>
      </div>
      <div class="clear-block"></div>
    </div>
  <?php endforeach; ?>
</div>
