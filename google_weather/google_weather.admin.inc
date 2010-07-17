<?php

function google_weather_block_save($delta, $edit) {
  variable_set("gw_location_{$delta}", $edit['gw_location']);
  variable_set("gw_days_{$delta}", $edit['gw_days']);
}

function google_weather_block_add_block_form() {
  module_load_include('inc', 'block', 'block.admin');
  return block_admin_configure($form_state, 'google_weather', NULL);
}

function google_weather_block_configure($delta, $edit) {
  $form_state = array('values' => google_weather_get_config($delta));
  return google_weather_configure_form($form_state);
}

function google_weather_get_config($delta = NULL) {
  $config = array(
    'delta' => $delta,
    'gw_location' => 'London',
    'gw_days' => 3,
  );

  if ($delta) {
    $config['gw_location']  = variable_get("gw_location_{$delta}",  'London');
    $config['gw_days'] = variable_get("gw_days_{$delta}", 3);
  }
  return $config;
}

function google_weather_get_title($delta = 0) {
  $config = google_weather_get_config($delta);
  return t("Weather for ") . $config['gw_location'];
}

function google_weather_block_list() {
  foreach(variable_get('gw_ids', array()) as $delta) {    
    $block[$delta]['info'] = google_weather_get_title($delta);
    $blocks[$delta]['cache'] = BLOCK_NO_CACHE;
  }
  return $block;
}

function google_weather_configure_form(&$form_state = NULL) {
  $config = array();

  if (!empty($form_state['values'])) {
    $config = $form_state['values'];
  }

  $config += google_weather_get_config();
  
  $form = array();

  $form['gw_location'] = array(
    '#type' => 'textfield',
    '#title' => t("Location"),
    '#default_value' => $config['gw_location'],
    '#required' => TRUE,
  );

  $form['gw_days'] = array(
    '#type' => 'select',
    '#title' => t("How long forecast do you want?"),
    '#default_value' => $config['gw_days'],
    '#options' => array(
      '2' => 1,
      '3' => 2,
      '4' => 3,
      '5' => 4,
    ),
    '#description' => t("Number of days, including today forecast"),
    '#required' => TRUE,
  );
  
  return $form;
}

function google_weather_block_add_block_form_submit($form, &$form_state) {
  $block_ids = variable_get('gw_ids', array());
  $delta = empty($block_ids) ? 1 : max($block_ids) + 1;

  $block_ids[] = $delta;
  variable_set('gw_ids', $block_ids);

  google_weather_block_save($delta, $form_state['values']);

  block_add_block_form_submit(NULL, $form_state);
}

function google_weather_delete(&$form_state, $delta = 0) {
  $title = google_weather_get_title($delta);
  $form['block_title'] = array('#type' => 'hidden', '#value' => $title);
  $form['delta'] = array('#type' => 'hidden', '#value' => $delta);
  
  return confirm_form($form, t('Are you sure you want to delete the "%name" block?', array('%name' => $title)), 'admin/build/block', NULL, t('Delete'), t('Cancel'));
}

function google_weather_delete_submit($form, &$form_state) {
  $delta = $form_state['values']['delta'];
  $block_ids = variable_get('gw_ids', array());
  unset($block_ids[array_search($delta, $block_ids)]);
  sort($block_ids);
  variable_set('menu_block_ids', $block_ids);
  variable_del("gw_location_{$delta}");
  variable_del("gw_days_{$delta}");

  db_query("DELETE FROM {blocks} WHERE module = 'google_weather' AND delta = %d", $delta);
  db_query("DELETE FROM {blocks_roles} WHERE module = 'google_weather' AND delta = %d", $delta);
  drupal_set_message(t('The "%name" block has been removed.', array('%name' => $form_state['values']['block_title'])));
  cache_clear_all();
  
  $form_state['redirect'] = 'admin/build/block';
  return;
}

function google_weather_block_list_alter(&$form, $form_state) {
  foreach (variable_get('gw_ids', array()) as $delta) {
    $form['google_weather_' . $delta]['delete'] = array('#value' => l(t('delete'), 'admin/build/block/delete-google-weather-block/'. $delta));
  }
}
